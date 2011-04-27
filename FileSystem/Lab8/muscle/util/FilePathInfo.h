/* This file is Copyright 2000-2009 Meyer Sound Laboratories Inc.  See the included LICENSE.txt file for details. */

#ifndef MuscleFilePathInfo_h
#define MuscleFilePathInfo_h

#ifndef WIN32
# include <sys/stat.h>
#endif

#include "system/SystemInfo.h"  // for GetFilePathSeparator()
#include "util/TimeUtilityFunctions.h"

namespace muscle {

/** A cross-platform API for examining the attributes of a particular file. */
class FilePathInfo
{
public:
   /** Default constructor:  creates an invalid FilePathInfo object.  */
   FilePathInfo() {Reset();}

   /** Constructor
     * @param filePath The path to the directory to open.  This is the same as calling SetFilePath(filePath).
     */
   FilePathInfo(const char * filePath) {SetFilePath(filePath);}

   /** Destructor.  Closes our held directory descriptor, if we have one. */
   ~FilePathInfo() {Reset();}

   /** Returns true iff something exists at the specified path. */
   bool Exists() const {return ((_flags & (1L<<FPI_FLAG_EXISTS))        != 0);}

   /** Returns true iff the item at the specified path is a regular data file. */
   bool IsRegularFile() const {return ((_flags & (1L<<FPI_FLAG_ISREGULARFILE)) != 0);}

   /** Returns true iff the item at the specified path is a directory. */
   bool IsDirectory()    const {return ((_flags & (1L<<FPI_FLAG_ISDIRECTORY))   != 0);}

   /** Returns the current size of the file */
   uint64 GetFileSize() const {return _size;}

   /** Returns the most recent access time, in microseconds since 1970.  Note that
     * not all filesystems update this time, so it may not be correct.
     */
   uint64 GetAccessTime() const {return _atime;}

   /** Returns the most recent modification time, in microseconds since 1970.  */
   uint64 GetModificationTime() const {return _mtime;}

   /** Returns the most creation time, in microseconds since 1970.  */
   uint64 GetCreationTime() const {return _ctime;}

   /** Sets this object's state to reflect the item that exists at (filePath)
     * @param filePath The path to examine.  SetFilePath(NULL) is the same as calling Reset().
     */
   void SetFilePath(const char * filePath)
   {
      int sLen = strlen(filePath);
      if ((sLen > 0)&&(filePath[sLen-1] == *GetFilePathSeparator()))
      {
         // Special case for paths that end in a slash:  we want to ignore the slash (otherwise we can't see files at this location, only folders)
         char * temp = newnothrow_array(char, sLen);  // not plus 1, because we're going to put the NUL where the trailing slash was
         if (temp)
         {
            memcpy(temp, filePath, sLen-1);
            temp[sLen-1] = '\0';
            SetFilePath(temp);
            delete [] temp;
            return;
         }
         else WARN_OUT_OF_MEMORY;
      }

      Reset();
      if (filePath)
      {
#ifdef WIN32
         // FILE_FLAG_BACKUP_SEMANTICS is necessary or CreateFile() will
         // fail when trying to open a directory.
         HANDLE hFile = CreateFileA(filePath, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
         if (hFile != INVALID_HANDLE_VALUE)
         {
            _flags |= (1L<<FPI_FLAG_EXISTS);

            BY_HANDLE_FILE_INFORMATION info;
            if (GetFileInformationByHandle(hFile, &info))
            {
               _flags |= (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? (1L<<FPI_FLAG_ISDIRECTORY) : (1L<<FPI_FLAG_ISREGULARFILE);
               _atime = InternalizeFileTime(info.ftLastAccessTime);
               _ctime = InternalizeFileTime(info.ftCreationTime);
               _mtime = InternalizeFileTime(info.ftLastWriteTime);
               _size  = (((uint64)info.nFileSizeHigh)<<32)|((uint64)info.nFileSizeLow);
            }
            CloseHandle(hFile);
         }
#else
# ifdef MUSCLE_64_BIT_PLATFORM
         struct stat64 statInfo;
         if (stat64(filePath, &statInfo) == 0)
# else
         struct stat statInfo;
         if (stat(filePath, &statInfo) == 0)
# endif
         {
            _flags |= (1L<<FPI_FLAG_EXISTS);
            if (S_ISDIR(statInfo.st_mode)) _flags |= (1L<<FPI_FLAG_ISDIRECTORY);
            if (S_ISREG(statInfo.st_mode)) _flags |= (1L<<FPI_FLAG_ISREGULARFILE);
            _size = statInfo.st_size;
# if defined(MUSCLE_64_BIT_PLATFORM) && !defined(_POSIX_SOURCE)
            _atime = InternalizeTimeSpec(statInfo.st_atimespec);
            _ctime = InternalizeTimeSpec(statInfo.st_birthtimespec);
            _mtime = InternalizeTimeSpec(statInfo.st_mtimespec);
# else
            _atime = InternalizeTimeT(statInfo.st_atime);
            _ctime = InternalizeTimeT(statInfo.st_ctime);
            _mtime = InternalizeTimeT(statInfo.st_mtime);
# endif
         }
#endif
      }
   }

   /** Resets this object to its default/invalid state */
   void Reset()
   {
      _flags = 0;
      _size  = 0;
      _atime = 0;
      _ctime = 0;
      _mtime = 0;
   }

private:
#ifdef WIN32
   uint64 InternalizeFileTime(const FILETIME & ft) const 
   {
      // subtract (1970-1601) to convert from Windows time base, in 100ns units
      const uint64 diffTime = ((uint64)116444736)*NANOS_PER_SECOND; 
      uint64 wft = (((uint64)ft.dwHighDateTime)<<32)|((uint64)ft.dwLowDateTime);
      if (wft <= diffTime) return 0;
      return ((wft-diffTime)/10);  // convert to MUSCLE-style microseconds
   }
#else
   uint64 InternalizeTimeSpec(const struct timespec & ts) const {return ((((uint64)ts.tv_sec)*MICROS_PER_SECOND)+(((uint64)ts.tv_nsec)/1000));}
   uint64 InternalizeTimeT(time_t t) const {return (t==((time_t)-1)) ? 0 : (((uint64)t)*MICROS_PER_SECOND);}
#endif

   enum { 
      FPI_FLAG_EXISTS = 0,
      FPI_FLAG_ISREGULARFILE,
      FPI_FLAG_ISDIRECTORY,
      NUM_FPI_FLAGS
   };

   uint32 _flags;
   uint64 _size;   // file size
   uint64 _atime;  // access time
   uint64 _ctime;  // creation time
   uint64 _mtime;  // modification time
};

}; // end namespace muscle

#endif
