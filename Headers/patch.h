#ifndef PATCH_H
#define PATCH_H

#include <QtGlobal>
#include <QCheckBox>
#include <QComboBox>
#include <QVector>
#include <cstring>
#include "file.h"

#define KOR 0
#define WAKOR 0

#define NOP 0xE1A00000
#define SHOPALWAYSOPEN 0xE3A00001

/* Obtained from patcher.h in Luma3DS: https://github.com/AuroraWright/Luma3DS/blob/master/sysmodules/loader/source/patcher.h */
#define MAKE_BRANCH(src,dst)      (0xEA000000 | (static_cast<quint32>(((reinterpret_cast<quint8 *>(dst) - reinterpret_cast<quint8 *>(src)) >> 2) - 2) & 0xFFFFFF))
#define MAKE_BRANCH_LINK(src,dst) (0xEB000000 | (static_cast<quint32>(((reinterpret_cast<quint8 *>(dst) - reinterpret_cast<quint8 *>(src)) >> 2) - 2) & 0xFFFFFF))

struct ShopCodeOffsets_s {
    quint16 off_NormalOpen;
    quint16 off_NightOpen;
    quint16 off_EarlyOpen;
    quint16 off_AlwaysOpen;
    bool   RequiresCustomCode;
    bool   HasNecessaryCode;
};

enum ShopTime : quint8 {
    NormalTime = 0,
    NightOwlTime = 1,
    EarlyBirdTime = 2
};

struct PatchValues {
    quint32 Value;
    quint32 OffsetFromBaseAddr;
};

extern ShopCodeOffsets_s off_Retail;
extern ShopCodeOffsets_s off_Nooklings[5];
extern ShopCodeOffsets_s off_Garden;
extern ShopCodeOffsets_s off_Ables;
extern ShopCodeOffsets_s off_Sham;
extern ShopCodeOffsets_s off_Kicks;
extern ShopCodeOffsets_s off_Nooks;
extern ShopCodeOffsets_s off_Katrina;
extern ShopCodeOffsets_s off_Redd;

//Search function from Nanquitas's CTRPF Utils.h
template <typename T>
static quint32     Search(const char* start, const int size, const QVector<T> &pattern)
{
    if (!start || !size || pattern.empty())
        return (0);

    const quint32   patternSize = pattern.size() * sizeof(T);
    const quint8    *patternc = reinterpret_cast<const quint8 *>(pattern.data());
    const quint8    *startPos = reinterpret_cast<const quint8 *>(start);

    quint32     table[256];

    for (quint32 i = 0; i < 256; i++)
        table[i] = patternSize;

    for (quint32 i = 0; i < patternSize - 1; i++)
        table[patternc[i]] = patternSize - i - 1;

    quint32 j = 0;
    while (j <= size - patternSize)
    {
        const quint8 c = startPos[j + patternSize - 1];
        if (patternc[patternSize - 1] == c && std::memcmp(patternc, startPos + j, patternSize - 1) == 0)
            return j;
        j += table[c];
    }
    return (0);
}

class Patch
{
public:

    Patch();

    Patch(quint32 NL_JPN, quint32 NL_USA, quint32 NL_EUR, quint32 NL_KOR,
          quint32 WA_JPN, quint32 WA_USA, quint32 WA_EUR, quint32 WA_KOR,
          QVector<PatchValues> Values);

    Patch(const QVector<quint8> Pattern, QVector<PatchValues> Values, quint32 OffsetFromPattern = 0);

    bool Apply(File* codebin, quint8 OR = 0);

    static void Init(void);

    quint32 m_Offset; //Offsets are code.bin + 0x100000, i.e. same offsets as hardware

private:
    QVector<PatchValues> m_Values;

};

extern Patch NativeFruitPrice;
extern Patch ReeseBuy;
extern Patch NooklingsBuy;
extern Patch LeilaBuy;
extern Patch NoResetti;
extern Patch Confetti;
extern Patch CherryBlossom;
extern Patch Weather;
extern Patch Season;
extern Patch CherryBTrees;
extern Patch AlwaysXmasTrees;
extern Patch Island3Nums;
extern Patch PickAllTours;
extern Patch MusicHasEcho;
//extern Patch NoGrassDecay;

/* Exefs->Player */
extern Patch PlayerSpeed;
extern Patch EditPattern;
extern Patch FlowersNoTrample;
extern Patch NoMosquito;

/* Exefs->Utilities */
extern Patch RegionCheck;
extern Patch ChecksumCheck;
extern Patch SecureValueCheck;

/* Shop Times: Addresses are start of function */
extern Patch Retail;
extern Patch Nooklings;
extern Patch Garden;
extern Patch Ables;
extern Patch Sham;
extern Patch Kicks;
extern Patch Nooks;
extern Patch Katrina;
extern Patch Redd;
extern Patch NightOwl;

#endif // PATCH_H
