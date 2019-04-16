// Copyright (c) 2019 The Zcoin Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ZCOIN_ZEROCOINWALLET_H
#define ZCOIN_ZEROCOINWALLET_H

#include <map>
#include "libzerocoin/Zerocoin.h"
#include "mintpool.h"
#include "uint256.h"
#include "primitives/zerocoin.h"
#include "wallet/wallet.h"


class CDeterministicMint;

class CZerocoinWallet
{
private:
    uint256 seedMaster;
    uint32_t nCountLastUsed;
    std::string strWalletFile;
    CMintPool mintPool;

public:
    CZerocoinWallet(std::string strWalletFile);

    void AddToMintPool(const std::pair<uint256, uint32_t>& pMint, bool fVerbose);
    bool SetMasterSeed(const uint256& seedMaster, bool fResetCount = false);
    uint256 GetMasterSeed() { return seedMaster; }
    void SyncWithChain(bool fGenerateMintPool = true);
    void GenerateDeterministicZerocoin(sigma::CoinDenominationV3 denom, sigma::PrivateCoinV3& coin, CDeterministicMint& dMint, bool fGenerateOnly = false);
    void GenerateMint(const uint32_t& nCount, const sigma::CoinDenominationV3 denom, sigma::PrivateCoinV3& coin, CDeterministicMint& dMint);
    void GetState(int& nCount, int& nLastGenerated);
    bool RegenerateMint(const CDeterministicMint& dMint, CZerocoinEntryV3& zerocoin);
    void GenerateMintPool(uint32_t nCountStart = 0, uint32_t nCountEnd = 0);
    bool LoadMintPoolFromDB();
    void RemoveMintsFromPool(const std::vector<uint256>& vPubcoinHashes);
    bool SetMintSeen(const GroupElement& bnValue, const int& nHeight, const uint256& txid, const sigma::CoinDenominationV3& denom);
    bool IsInMintPool(const GroupElement& bnValue) { return mintPool.Has(bnValue); }
    void Lock();
    void SeedToZerocoin(const uint512& seedZerocoin, GroupElement& bnValue, sigma::PrivateCoinV3& coin);
    bool CheckSeed(const CDeterministicMint& dMint);
    // Count updating functions
    uint32_t GetCount();
    void SetCount(uint32_t nCount);
    void UpdateCountLocal();
    void UpdateCountDB();
    void UpdateCount();

private:
    uint512 GetZerocoinSeed(uint32_t n);
};

#endif //ZCOIN_ZEROCOINWALLET_H