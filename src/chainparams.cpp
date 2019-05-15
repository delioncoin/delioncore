// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX Developers
// Copyright (c) 2018-2019 The Delion Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions

static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0xeaaaea7d13c08bff5ba2be83cb558de02a5f872af48a67a9802dd4bc95f0a399"))
    (50000,uint256("06466ca82b589f87682a84be2055ab487a5b305b10b3ff89db70fed9b169b34c"))
    (100000,uint256("870b056fde9975f0b8d81eb800dd7533b446095d26518acdadce098c65b5a964"))
    (140000,uint256("ec916ebba4184b3936d081cff5a3fad56a80672c9f71c359c83d8f96ba97fc33"))
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1557117733, // * UNIX timestamp of last checkpoint block
    295778,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    5000       // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1537607775,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1537607764,
    0,
    0};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x18;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x04;
        pchMessageStart[3] = 0x57;
        vAlertPubKey = ParseHex("0480e0e72c17b6205828d44d080648e9c27ab1e2a17ecbaf80962e4f6c2c89ac42862352bf459271886b752cb64784e8b66c01341a9973ada2a7c3b1dc99d32631");
        nDefaultPort = 15858;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Delion starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Delion: 60 seconds
        nTargetSpacingSlowLaunch = 1 * 60;
        nTargetSpacing = 1 * 60;  // Delion: 60 Seconds
        nMaturity = 20;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 1000000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 600;
        nModifierUpdateBlock = 1;
        nZerocoinStartHeight = 601;
        nAccumulatorStartHeight = 1;
        nZerocoinStartTime = 1537607855; // 1537760280
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint

        const char* pszTimestamp = "Delion Project Started";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 1 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04c1eb45c93f2d5c6857211458fd588901af54903651b4a21921cdf11e702c9ebecddbbbbfe8dc1fb9e79678536720bab21f3a3755160e93d9beaa69acbb1e9b14") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1537607855;
        genesis.nBits = 0x1e0fffff;
        genesis.nNonce = 594490584;
        
        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0xeaaaea7d13c08bff5ba2be83cb558de02a5f872af48a67a9802dd4bc95f0a399"));
        assert(genesis.hashMerkleRoot == uint256("0x3540db09006953f2ef04edce58acf06c8f003cb7d2632c8f664cb5e086933fd6"));
		
		vFixedSeeds.clear();
		vSeeds.clear();
		
        vSeeds.push_back(CDNSSeedData("1.delion.online", "1.delion.online"));
        vSeeds.push_back(CDNSSeedData("2.delion.online", "2.delion.online"));
        vSeeds.push_back(CDNSSeedData("3.delion.online", "3.delion.online"));
        vSeeds.push_back(CDNSSeedData("4.delion.online", "4.delion.online"));
        vSeeds.push_back(CDNSSeedData("5.delion.online", "5.delion.online"));
        vSeeds.push_back(CDNSSeedData("6.delion.online", "6.delion.online"));
        vSeeds.push_back(CDNSSeedData("7.delion.online", "7.delion.online"));
        vSeeds.push_back(CDNSSeedData("8.delion.online", "8.delion.online"));
        vSeeds.push_back(CDNSSeedData("9.delion.online", "9.delion.online"));
        vSeeds.push_back(CDNSSeedData("10.delion.online", "10.delion.online"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 31); // D
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 48); // L
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 53); // N
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0xb1)(0x9c)(0x11)(0x8c).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x7f)(0xfd)(0xb7)(0xcc).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x10)(0xb8)(0xa4)(0x8a).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));


		fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "0484698d3ba6ba6e7423fa5cbd6a89e0a9a5348f88d332b44a5cb1a8b7ed2c1eaa335fc8dc4f012cb8241cc0bdafd6ca70c5f5448916e4e6f511bcd746ed57dc50";
        strObfuscationPoolDummyAddress  = "D87q2gC9j6nNrnzCsg4aY6bHMLsT9nUhEw";
        nStartMasternodePayments = 1403728576; //Wed, 25 Jun 2014 20:36:16 GMT
        
        /** Zerocoin */
        zerocoinModulus = "0xc95577b6dce0049b0a20c779af38079355abadde1a1d80c353f6cb697a7ae5a087bad39caa5798478551d0f9d91e6267716506f32412de1d19d17588765eb9502b85c6a18abdb05791cfd8b734e960281193705eeece210920cc922b3af3ceb178bf12c22eb565d5767fbf19545639be8953c2c38ffad41f3371e4aac750ac2d7bd614b3faabb453081d5d88fdbb803657a980bc93707e4b14233a2358c97763bf28f7c933206071477e8b371f229bc9ce7d6ef0ed7163aa5dfe13bc15f7816348b328fa2c1e69d5c88f7b94cee7829d56d1842d77d7bb8692e9fc7b7db059836500de8d57eb43c345feb58671503b932829112941367996b03871300f25efb5";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * ZCENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x18;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x04;
        pchMessageStart[3] = 0x57;
        vAlertPubKey = ParseHex("04c1eb45c93f2d5c6857211458fd588901af54903651b4a21921cdf11e702c9ebecddbbbbfe8dc1fb9e79678536720bab21f3a3755160e93d9beaa69acbb1e9b14");
        nDefaultPort = 25858;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // Delion: 1 day
        nTargetSpacing = 1 * 60;  // Delion: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1524711188;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1537602753;
        genesis.nNonce = 531957;
        
                       
        hashGenesisBlock = genesis.GetHash();
		assert(hashGenesisBlock == uint256("0x881ec759a95bee06f8b76147ef6e2e99ff487eb92062132dc74055a09d5ef7c6"));

        vFixedSeeds.clear();
        vSeeds.clear();
        
        vSeeds.push_back(CDNSSeedData("test.delion.online", "test.delion.online"));


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 126); // Testnet delion addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet delion script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet delion BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet delion BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet delion BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04188441e39d99aa69068ee07d26980f459b84465bbd765c6ee15d1aec5b76b5aebb01b24be184a1d3a12af61276549d96cc9499d909f8afc183132837d18d643d";
        strObfuscationPoolDummyAddress = "xp87cG8UEQgzs1Bk67Yk884C7pnQfAeo7q";
        nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x18;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x04;
        pchMessageStart[3] = 0x57;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // Delion: 1 day
        nTargetSpacing = 1 * 60;        // Delion: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1537602753;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 2266010;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 35858;
        assert(hashGenesisBlock == uint256("0x6ae8a8b69440f2ffe28e2e16b50f4d8904877b08ba971e9324a52adf049deb3a"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 45858;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
