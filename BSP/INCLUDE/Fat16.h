Uint32 	FirstSectorofCluster16( Uint16 n );
Uint16 	ThisFatSecNum16( Uint16 clusterNum );
Uint16 	ThisFatEntOffset16( Uint16 clusterNum );
Uint16 	GetNextClusterNum16( Uint16 clusterNum );
Uint16 	GetClusterNumFromSectorNum( Uint32 sectorNum );

Uint16 	GoToPointer16( Uint32 pointer );
Uint16 	GetFreeCusterNum16( void );
Uint16 	CreateClusterLink16( Uint16 currentCluster );
Uint16 	DeleteClusterLink16( Uint16 clusterNum );

