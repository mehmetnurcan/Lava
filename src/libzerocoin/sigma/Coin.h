#ifndef ZCOIN_SIGMA_COIN_H
#define ZCOIN_SIGMA_COIN_H

#include "SigmaPrimitives.h"
#include <libzerocoin/Zerocoin.h>
#include "Params.h"
#include "consensus/validation.h"

namespace sigma {


enum class CoinDenominationV3 {
    SIGMA_DENOM_0_1 = 0,
    SIGMA_DENOM_0_5 = 1,

    SIGMA_DENOM_1 = 2,
    SIGMA_DENOM_10 = 3,
    SIGMA_DENOM_100 = 4
};

// for LogPrintf.
std::ostream& operator<<(std::ostream& stream, CoinDenominationV3 denomination);

// Functions to convert denominations to/from an integer value.
bool DenominationToInteger(CoinDenominationV3 denom, int64_t& denom_out, CValidationState &state);
bool IntegerToDenomination(int64_t value, CoinDenominationV3& denom_out, CValidationState &state);
bool DenominationToInteger(CoinDenominationV3 denom, int64_t& denom_out);
bool IntegerToDenomination(int64_t value, CoinDenominationV3& denom_out);
bool StringToDenomination(const std::string& str, CoinDenominationV3& denom_out);
bool RealNumberToDenomination(const double& value, CoinDenominationV3& denom_out);

class PublicCoinV3 {
public:
    PublicCoinV3();

    PublicCoinV3(const GroupElement& coin, const CoinDenominationV3 d);

    const GroupElement& getValue() const;
    CoinDenominationV3 getDenomination() const;

    bool operator==(const PublicCoinV3& other) const;
    bool operator!=(const PublicCoinV3& other) const;
    bool validate() const;
    size_t GetSerializeSize(int nType, int nVersion) const;

    template<typename Stream>
    inline void Serialize(Stream& s, int nType, int nVersion) const {
        int size = value.memoryRequired();
        unsigned char buffer[size + sizeof(int32_t)];
        value.serialize(buffer);
        std::memcpy(buffer + size, &denomination, sizeof(denomination));
        char* b = (char*)buffer;
        s.write(b, size + sizeof(int32_t));
    }

    template<typename Stream>
    inline void Unserialize(Stream& s, int nType, int nVersion) {
        int size = value.memoryRequired();
        unsigned char buffer[size + sizeof(int32_t)];
        char* b = (char*)buffer;
        s.read(b, size + sizeof(int32_t));
        value.deserialize(buffer);
        std::memcpy(&denomination, buffer + size, sizeof(denomination));
    }

// private: TODO(martun): change back to private
    GroupElement value;
    CoinDenominationV3 denomination;
};

class PrivateCoinV3{
public:
    template<typename Stream>
    PrivateCoinV3(const ParamsV3* p, Stream& strm): params(p), publicCoin() {
        strm >> *this;
    }

    PrivateCoinV3(const ParamsV3* p,
        CoinDenominationV3 denomination = CoinDenominationV3::SIGMA_DENOM_1, 
        int version = ZEROCOIN_TX_VERSION_3);

    const PublicCoinV3& getPublicCoin() const;
    const Scalar& getSerialNumber() const;
    const Scalar& getRandomness() const;
    unsigned int getVersion() const;
    void setPublicCoin(const PublicCoinV3& p);
    void setRandomness(Scalar n);
    void setSerialNumber(Scalar n);
    void setVersion(unsigned int nVersion);
    const unsigned char* getEcdsaSeckey() const;

    void setEcdsaSeckey(const vector<unsigned char> &seckey); 

    static const Scalar serialNumberFromSerializedPublicKey(
        secp256k1_context *context,
        secp256k1_pubkey *pubkey);

private:
    const ParamsV3* params;
    PublicCoinV3 publicCoin;
    Scalar randomness;
    Scalar serialNumber;
    unsigned int version = 0;
    unsigned char ecdsaSeckey[32];

    void mintCoin(const CoinDenominationV3 denomination);

};

}// namespace sigma

#endif //ZCOIN_SIGMA_COIN_H
