/*
**  Copyright (c) 2013-2016 Mahmoud Fayed <msfclipper@yahoo.com> 
**  Include Files 
*/
#include "ring.h"
#include "sqlite3.h"
/* Data */
typedef struct ring_sqlite {
	sqlite3 *db  ;
} ring_sqlite ;
/* Functions */

void ring_vm_sqlite_loadfunctions ( RingState *pRingState )
{
	ring_vm_funcregister("sqlite_init",ring_vm_sqlite_init);
	ring_vm_funcregister("sqlite_close",ring_vm_sqlite_close);
	ring_vm_funcregister("sqlite_open",ring_vm_sqlite_open);
	ring_vm_funcregister("sqlite_errmsg",ring_vm_sqlite_errmsg);
}

void ring_vm_sqlite_init ( void *pPointer )
{
	ring_sqlite *psqlite  ;
	psqlite = (ring_sqlite *) malloc(sizeof(ring_sqlite)) ;
	if ( psqlite == NULL ) {
		RING_API_ERROR(RING_OOM);
		return ;
	}
	psqlite->db = NULL ;
	RING_API_RETCPOINTER(psqlite,RING_VM_POINTER_SQLITE);
}

void ring_vm_sqlite_close ( void *pPointer )
{
	ring_sqlite *psqlite  ;
	if ( RING_API_PARACOUNT != 1 ) {
		RING_API_ERROR(RING_API_MISS1PARA);
		return ;
	}
	if ( RING_API_ISPOINTER(1) ) {
		psqlite = (ring_sqlite *) RING_API_GETCPOINTER(1,RING_VM_POINTER_SQLITE) ;
		if ( psqlite == NULL ) {
			return ;
		}
		if ( psqlite->db ) {
			sqlite3_close(psqlite->db);
		}
		free( psqlite ) ;
		RING_API_SETNULLPOINTER(1);
		RING_API_RETNUMBER(1);
	} else {
		RING_API_ERROR(RING_API_BADPARATYPE);
	}
}

void ring_vm_sqlite_open ( void *pPointer )
{
	ring_sqlite *psqlite  ;
	int rc  ;
	if ( RING_API_PARACOUNT != 2 ) {
		RING_API_ERROR(RING_API_MISS2PARA);
		return ;
	}
	if ( RING_API_ISPOINTER(1) && RING_API_ISSTRING(2) ) {
		psqlite = (ring_sqlite *) RING_API_GETCPOINTER(1,RING_VM_POINTER_SQLITE) ;
		if ( psqlite == NULL ) {
			return ;
		}
		if ( psqlite->db ) {
			rc = sqlite3_open(RING_API_GETSTRING(1),&psqlite->db);
			RING_API_RETNUMBER((double) rc);
		}
	} else {
		RING_API_ERROR(RING_API_BADPARATYPE);
	}
}

void ring_vm_sqlite_errmsg ( void *pPointer )
{
	ring_sqlite *psqlite  ;
	if ( RING_API_PARACOUNT != 1 ) {
		RING_API_ERROR(RING_API_MISS1PARA);
		return ;
	}
	if ( RING_API_ISPOINTER(1) ) {
		psqlite = (ring_sqlite *) RING_API_GETCPOINTER(1,RING_VM_POINTER_SQLITE) ;
		if ( psqlite == NULL ) {
			return ;
		}
		if ( psqlite->db ) {
			RING_API_RETSTRING(sqlite3_errmsg(psqlite->db));
		}
	} else {
		RING_API_ERROR(RING_API_BADPARATYPE);
	}
}