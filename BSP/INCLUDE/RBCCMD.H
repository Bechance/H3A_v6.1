#define MAX_par 							15
#define RBC_CMD_READ10						0x28
#define RBC_CMD_READCAPACITY				0x25
#define RBC_CMD_WRITE10						0x2A
// RBC commands
#define SPC_CMD_INQUIRY						0x12
#define SPC_CMD_PRVENTALLOWMEDIUMREMOVAL	0x1E
#define SPC_CMD_REQUESTSENSE				0x03
#define SPC_CMD_TESTUNITREADY				0x00
////////////////////////////////////////////////////////////////////////////////////
// Command Descriptor Block
//      _RBC : Reduced Block Command
//      _SPC : SPC-2 SCSI primary Command - 2
////////////////////////////////////////////////////////////////////////////////////
typedef struct _SYS_INFO_BLOCK
{
	Uint32 StartSector;
	Uint32 TotalSector;

	Uint16 BPB_BytesPerSec;
	Uint16 BPB_SecPerClus;

	Uint16 BPB_NumFATs;
	Uint16 BPB_RootEntCnt;
	Uint16 BPB_TotSec16;
	Uint16 BPB_FATSz16;
	Uint16 BPB_SecPerTrk;
	Uint16 BPB_NumHeads;
	Uint32 BPB_HiddSec;
	Uint32 BPB_TotSec32;
///////////////////////////////
	Uint32 FatStartSector;
	Uint32 RootStartSector;
	Uint32 FirstDataSector;
/////////////////////FAT32///////////////////////////////
	Uint32 BPB_FATSz32;
	Uint32 RootStartCluster;
	Uint32 TotCluster;
	Uint16 FAT;
} SYS_INFO_BLOCK;

typedef struct _FILE_INFO
{
	Uint16 bFileOpen;
	Uint32 StartCluster;//unsigned int StartCluster;
	Uint32 LengthInByte;
	Uint32 ClusterPointer;//unsigned int ClusterPointer;
	Uint32 SectorPointer;
	Uint16 OffsetofSector;
	Uint16 SectorofCluster;
	Uint32 pointer;
	Uint32 FatSectorPointer;//unsigned int	FatSectorPointer;
} FILE_INFO;

typedef struct _DIR_INFO
{
	Uint16 	name[8];
	Uint16 	extension[3];
	Uint16 	attribute;
	Uint16 	Reserved[10];
	Uint16 	lastUpdateDate;
	Uint16 	lastUpdateTime;
	Uint16 	startCluster;
	Uint32 	length;
} DIR_INFO;

