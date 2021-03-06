/*
 *   Copyright (c) 1998-2004 T. Kamei (kamei@jsdlab.co.jp)
 *
 *   Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose is hereby granted provided
 * that the above copyright notice and this permission notice appear
 * in all copies of the software and related documentation.
 *
 *                          NO WARRANTY
 *
 *   THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY WARRANTIES;
 * WITHOUT EVEN THE IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS
 * FOR A PARTICULAR PURPOSE.
 */

#ifndef _UNRARAPI_H_
#define _UNRARAPI_H_

extern "C"
{
#include "UnRAR.h"
}

#define FRAR_PREVVOL 1
#define FRAR_NEXTVOL 2
#define FRAR_ENCRYPTED 4
#define FRAR_COMMENT 8
#define FRAR_SOLID 16

#define FRAR_PATH_MAX (sizeof ((RARHeaderDataEx *)0)->FileName)

class rarOpenArchiveData: public RAROpenArchiveData
{
public:
  rarOpenArchiveData (char *buf = 0, int size = 0)
    {
      CmtBuf = buf;
      CmtBufSize = size;
    }
  rarOpenArchiveData (const char *path, int mode,
                      char *buf = 0, int size = 0)
    {
      ArcName = (char *)path;
      CmtBuf = buf;
      CmtBufSize = size;
      OpenMode = mode;
    }
};

class rarHeaderData: public RARHeaderDataEx
{
public:
  rarHeaderData ()
    {
      CmtBuf = 0;
      CmtBufSize = 0;
    }
  rarHeaderData (char *buf, int size)
    {
      CmtBuf = buf;
      CmtBufSize = size;
    }
};

typedef HANDLE (__stdcall *RAROPENARCHIVE)(RAROpenArchiveData *);
typedef int (__stdcall *RARCLOSEARCHIVE)(HANDLE);
typedef int (__stdcall *RARREADHEADER)(HANDLE, RARHeaderData *);
typedef int (__stdcall *RARREADHEADEREX)(HANDLE, RARHeaderDataEx *);
typedef int (__stdcall *RARPROCESSFILE)(HANDLE, int, char *, char *);
typedef void (__stdcall *RARSETCALLBACK)(HANDLE,int (CALLBACK*)(UINT,LPARAM,LPARAM,LPARAM),LPARAM);

#ifndef EXTERN
#define EXTERN extern
#endif

EXTERN RAROPENARCHIVE rarOpenArchive;
EXTERN RARCLOSEARCHIVE rarCloseArchive;
EXTERN RARREADHEADER rarReadHeader;
EXTERN RARREADHEADEREX rarReadHeaderEx;
EXTERN RARPROCESSFILE rarProcessFile;
EXTERN RARSETCALLBACK rarSetCallback;

class rarData
{
public:
  HANDLE h;
  rarOpenArchiveData oad;
  rarHeaderData hd;

  LPVOID pUserData;
  bool can_ask_password;
  bool is_missing_password;

  rarData ()
       : h (0),pUserData(NULL),can_ask_password(true),is_missing_password(false)
    {}
  ~rarData ()
    {close ();}
  bool open (const char *filename, int mode, char *buf = 0, int size = 0);
  int close ();
  int read_header ()
    {return rarReadHeaderEx (h, &hd);}
  int skip () const
    {return rarProcessFile (h, RAR_SKIP, 0, 0);}
  int test () const
    {return rarProcessFile (h, RAR_TEST, 0, 0);}
  int extract (const char *path, const char *name) const
    {return rarProcessFile (h, RAR_EXTRACT, (char *)path, (char *)name);}
};

HINSTANCE load_rarapi ();
bool file_executable_p (const char *path);
int calc_ratio (u_long comp_sz_high, u_long comp_sz_low, u_long orig_sz_high, u_long orig_sz_low);
union int64;
int calc_ratio (const int64 &comp_sz, const int64 &orig_sz);
const char *method_string (int method);
int os_type (int os);
const char *attr_string (int attr);

#endif
