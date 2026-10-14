--==============================================================================
-- Pegasus - Daily Rewards and Collection persistence
--
-- Run once against the MuOnline database.
--
-- Both systems hang off the account rather than the character: the calendar is
-- an attendance streak for the person logging in, not for whichever character
-- they happened to pick. MEMB_INFO is the account table; Character is per
-- character and would let one account farm the calendar once per character.
--
-- Safe to run more than once - every statement checks first.
--==============================================================================

USE [MuOnline]
GO

--------------------------------------------------------------------------------
-- Daily Rewards
--
--   DailyDay        0..27, which day of the calendar is currently claimable
--   DailyClaimed    bitmask, bit n set = day n already taken
--   DailyLastDate   YYYYMMDD of the last rollover, as a plain int so the
--                   comparison needs no timezone or DST arithmetic
--------------------------------------------------------------------------------

IF NOT EXISTS (SELECT * FROM syscolumns
               WHERE id = OBJECT_ID('MEMB_INFO') AND name = 'DailyDay')
BEGIN
    ALTER TABLE [dbo].[MEMB_INFO] ADD [DailyDay] tinyint NOT NULL DEFAULT (0)
END
GO

IF NOT EXISTS (SELECT * FROM syscolumns
               WHERE id = OBJECT_ID('MEMB_INFO') AND name = 'DailyClaimed')
BEGIN
    ALTER TABLE [dbo].[MEMB_INFO] ADD [DailyClaimed] int NOT NULL DEFAULT (0)
END
GO

IF NOT EXISTS (SELECT * FROM syscolumns
               WHERE id = OBJECT_ID('MEMB_INFO') AND name = 'DailyLastDate')
BEGIN
    ALTER TABLE [dbo].[MEMB_INFO] ADD [DailyLastDate] int NOT NULL DEFAULT (0)
END
GO

--------------------------------------------------------------------------------
-- Collection
--
-- 6 sets x 12 slots = 72 bits, so it does not fit one int. Three columns keep
-- it queryable; a binary(9) would be tighter but unreadable in Management
-- Studio when something goes wrong, and this is a table you will be looking at
-- by hand while wiring it up.
--------------------------------------------------------------------------------

IF NOT EXISTS (SELECT * FROM syscolumns
               WHERE id = OBJECT_ID('MEMB_INFO') AND name = 'CollectionMask0')
BEGIN
    ALTER TABLE [dbo].[MEMB_INFO] ADD [CollectionMask0] int NOT NULL DEFAULT (0)
END
GO

IF NOT EXISTS (SELECT * FROM syscolumns
               WHERE id = OBJECT_ID('MEMB_INFO') AND name = 'CollectionMask1')
BEGIN
    ALTER TABLE [dbo].[MEMB_INFO] ADD [CollectionMask1] int NOT NULL DEFAULT (0)
END
GO

IF NOT EXISTS (SELECT * FROM syscolumns
               WHERE id = OBJECT_ID('MEMB_INFO') AND name = 'CollectionMask2')
BEGIN
    ALTER TABLE [dbo].[MEMB_INFO] ADD [CollectionMask2] int NOT NULL DEFAULT (0)
END
GO

--------------------------------------------------------------------------------
-- Read and write, so the DataServer talks to one contract instead of building
-- SQL strings. Changing storage later then means changing these two, not the
-- server source.
--------------------------------------------------------------------------------

IF OBJECT_ID('WZ_PegasusProgressLoad') IS NOT NULL
    DROP PROCEDURE [dbo].[WZ_PegasusProgressLoad]
GO

CREATE PROCEDURE [dbo].[WZ_PegasusProgressLoad]
    @AccountID varchar(10)
AS
BEGIN
    SET NOCOUNT ON

    SELECT
        ISNULL(DailyDay, 0)         AS DailyDay,
        ISNULL(DailyClaimed, 0)     AS DailyClaimed,
        ISNULL(DailyLastDate, 0)    AS DailyLastDate,
        ISNULL(CollectionMask0, 0)  AS CollectionMask0,
        ISNULL(CollectionMask1, 0)  AS CollectionMask1,
        ISNULL(CollectionMask2, 0)  AS CollectionMask2
    FROM [dbo].[MEMB_INFO]
    WHERE memb___id = @AccountID
END
GO

IF OBJECT_ID('WZ_PegasusProgressSave') IS NOT NULL
    DROP PROCEDURE [dbo].[WZ_PegasusProgressSave]
GO

CREATE PROCEDURE [dbo].[WZ_PegasusProgressSave]
    @AccountID       varchar(10),
    @DailyDay        tinyint,
    @DailyClaimed    int,
    @DailyLastDate   int,
    @CollectionMask0 int,
    @CollectionMask1 int,
    @CollectionMask2 int
AS
BEGIN
    SET NOCOUNT ON

    UPDATE [dbo].[MEMB_INFO]
    SET DailyDay        = @DailyDay,
        DailyClaimed    = @DailyClaimed,
        DailyLastDate   = @DailyLastDate,
        CollectionMask0 = @CollectionMask0,
        CollectionMask1 = @CollectionMask1,
        CollectionMask2 = @CollectionMask2
    WHERE memb___id = @AccountID
END
GO

--------------------------------------------------------------------------------
-- Verify
--------------------------------------------------------------------------------

SELECT name AS AddedColumn
FROM syscolumns
WHERE id = OBJECT_ID('MEMB_INFO')
  AND name IN ('DailyDay','DailyClaimed','DailyLastDate',
               'CollectionMask0','CollectionMask1','CollectionMask2')
ORDER BY name
GO
