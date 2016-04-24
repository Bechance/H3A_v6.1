//////////////////////////////////////////////////////////////////////
Uint32 FirstSectorofCluster32( Uint32 clusterNum );
Uint32 ThisFatSecNum32( Uint32 clusterNum );
Uint32 ThisFatEntOffset32( Uint32 clusterNum );
Uint32 GetNextClusterNum32( Uint32 clusterNum );
Uint16 GoToPointer32( Uint32 pointer );
Uint16 DeleteClusterLink32( Uint32 clusterNum );
Uint32 GetFreeCusterNum32( void );
Uint32 CreateClusterLink32( Uint32 currentCluster );
