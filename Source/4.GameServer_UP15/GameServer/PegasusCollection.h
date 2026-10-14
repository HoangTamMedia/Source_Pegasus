#pragma once

#include "User.h"

// Item collection, server side.
//
// The client models this in PegasusCollection.h: 6 sets of 12 slots, each slot
// holding one item, each set granting a percent bonus once complete. This
// mirrors that shape and owns the two things the client must not decide - what
// is actually in the collection, and whether the item being handed in is the
// one the pedestal asked for.
//
// Same rule as the daily calendar: the client says "I am donating the item in
// inventory slot 9 to set 2 slot 5". The server looks at that inventory slot
// itself, matches it against its own table, consumes it, and only then sets the
// bit. It never trusts a client-supplied item id.

#define COLL_SET_COUNT   6
#define COLL_SLOT_COUNT  12

// 6 sets x 12 slots = 72 bits, which is why this is three DWORDs rather than
// one. Stored and sent as a unit so a set can never be half-updated.
struct COLLECTION_MASK
{
	DWORD Bits[3];
};

// Server -> client. The whole collection, for the same reason the daily sync is
// whole: 12 bytes costs nothing and removes a class of desync bug.
struct PMSG_COLLECTION_SYNC
{
	PBMSG_HEAD2 header;		// C1:F3:70
	COLLECTION_MASK Mask;
};

// Client -> server. Inventory slot, not item id - the server reads the item out
// of the player's own inventory so a forged id buys nothing.
struct PMSG_COLLECTION_DONATE_REQ
{
	PBMSG_HEAD2 header;		// C1:F3:71
	BYTE Set;
	BYTE Slot;
	BYTE InventorySlot;
	BYTE pad;
};

struct PMSG_COLLECTION_DONATE_RESULT
{
	PBMSG_HEAD2 header;		// C1:F3:72
	BYTE  Result;			// see eCollectionResult
	BYTE  Set;
	BYTE  Slot;
	BYTE  pad;
	COLLECTION_MASK Mask;
};

enum eCollectionResult
{
	COLL_OK = 0,
	COLL_ERR_RANGE,			// set or slot out of bounds
	COLL_ERR_ALREADY,		// pedestal already filled
	COLL_ERR_WRONG_ITEM,	// the inventory slot does not hold what it wants
	COLL_ERR_NO_ITEM,		// empty inventory slot
	COLL_ERR_DISABLED,
};

// One row of Data\Custom\Collection.txt.
struct COLLECTION_SLOT_INFO
{
	int ItemType;
	int ItemIndex;
	int ItemLevel;			// -1 = any level accepted
	int Rarity;				// 0..3, picks the frame the client draws
};

struct COLLECTION_SET_INFO
{
	int  Bonus;				// percent, applied once every slot is filled
	int  BonusType;			// which stat the bonus feeds
	COLLECTION_SLOT_INFO Slot[COLL_SLOT_COUNT];
};

class CPegasusCollection
{
public:
	CPegasusCollection();
	virtual ~CPegasusCollection();

	void Init();
	void Load(char* path);

	void SendSync(int aIndex);
	void DonateRequest(int aIndex, BYTE Set, BYTE Slot, BYTE InventorySlot);

	// True once every slot in the set is filled. This is what the bonus hangs
	// off, and what the client's "4 / 12 collected" line counts toward.
	bool IsSetComplete(LPOBJ lpObj, int Set) const;

	// Total bonus from every completed set, for the stat calculation to fold in.
	int  GetTotalBonus(LPOBJ lpObj, int BonusType) const;

	static bool GetBit(const COLLECTION_MASK* pMask, int Set, int Slot);
	static void SetBit(COLLECTION_MASK* pMask, int Set, int Slot);

private:
	void SendResult(int aIndex, BYTE Result, BYTE Set, BYTE Slot);

	bool m_bEnabled;
	COLLECTION_SET_INFO m_Set[COLL_SET_COUNT];
};

extern CPegasusCollection gPegasusCollection;
