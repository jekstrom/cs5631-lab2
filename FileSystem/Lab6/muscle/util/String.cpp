/* This file is Copyright 2000-2009 Meyer Sound Laboratories Inc.  See the included LICENSE.txt file for details. */  

#include "util/String.h"
#include <stdarg.h>

namespace muscle {

status_t
String::SetFromString(const String & s, uint32 firstChar, uint32 afterLastChar)
{
   afterLastChar = muscleMin(afterLastChar, s.Length());
   uint32 len = (afterLastChar > firstChar) ? (afterLastChar-firstChar) : 0;
   if (len > 0)
   {
      if (EnsureBufferSize(len+1, false) != B_NO_ERROR) return B_ERROR;
      if (&s == this) memmove(_buffer, s()+firstChar, len);
                 else memcpy( _buffer, s()+firstChar, len);
      _buffer[len] = '\0';
      _length = len;
   }
   else Clear();

   return B_NO_ERROR;
}

status_t
String::SetCstr(const char * str, uint32 maxLen)
{
   // If (str)'s got a NUL byte before maxLen, make (maxLen) smaller.
   // We can't call strlen(str) because we don't have any guarantee that the NUL 
   // byte even exists!  Without a NUL byte, strlen() could run off into the weeds...
   uint32 sLen = 0;
   if (str) {while((sLen<maxLen)&&(str[sLen] != '\0')) sLen++;}
   if (sLen < maxLen) maxLen = sLen;
   if (maxLen > 0)
   {
      if (str[maxLen-1] != '\0') maxLen++;  // make room to add the NUL byte if necessary
      if (EnsureBufferSize(maxLen, false) != B_NO_ERROR) return B_ERROR;
      if (muscleInRange((char *)str, _buffer, &_buffer[_length])) memmove(_buffer, str, maxLen-1);
                                                             else memcpy(_buffer, str, maxLen-1);
      _buffer[maxLen-1] = '\0';
      _length = maxLen-1;
   }
   else Clear();

   return B_NO_ERROR;
}

String &
String::operator+=(const String &other)
{
   uint32 otherLen = other.Length();
   if ((otherLen > 0)&&(EnsureBufferSize(Length() + otherLen + 1, true) == B_NO_ERROR))
   {
      memcpy(&_buffer[_length], other(), other.Length()+1);
      _length += otherLen;
   }
   return *this;
}

String &
String::operator+=(const char * other)
{
   if (other == NULL) other = "";
   uint32 otherLen = (uint32) strlen(other);
   if (otherLen > 0)
   {
           if (muscleInRange(other, (const char *)_buffer, (const char *)(_buffer+_length))) return operator+=(String(other));  // avoid self-entanglement
      else if (EnsureBufferSize(Length() + otherLen + 1, true) == B_NO_ERROR)
      {
         memcpy(&_buffer[_length], other, otherLen+1);
         _length += otherLen;
      }
   }
   return *this;
}

String &
String::operator-=(const char aChar)
{
   int idx = LastIndexOf(aChar);
   if (idx >= 0)
   {
      memmove(&_buffer[idx], &_buffer[idx+1], _length-idx);
      --_length;
   }
   return *this;
}

String &
String::operator-=(const String &other)
{
        if (*this == other) Clear();
   else if (other.HasChars())
   {
      int idx = LastIndexOf(other);
      if (idx >= 0)
      {
         uint32 newEndIdx = idx+other.Length();
         memmove(&_buffer[idx], &_buffer[newEndIdx], 1+_length-newEndIdx);
         _length -= other.Length();
      }
   }
   return *this;
}

String &
String::operator-=(const char * other)
{
   int otherLen = ((other)&&(*other)) ? (int)strlen(other) : 0;
   if (otherLen > 0)
   {
      int idx = LastIndexOf(other);
      if (idx >= 0)
      {
         uint32 newEndIdx = idx+otherLen;
         memmove(&_buffer[idx], &_buffer[newEndIdx], 1+_length-newEndIdx);
         _length -= otherLen;
      }
   }
   return *this;
}

String &
String::operator<<(int rhs)
{
   char buff[64];
   sprintf(buff, "%d", rhs);
   return *this << buff;
}

String &
String::operator<<(float rhs)
{
   char buff[64];
   sprintf(buff, "%.2f", rhs);
   return *this << buff;
}

String &
String::operator<<(bool rhs)
{
   const char* val = rhs ? "true" : "false"; 
   return *this << val;
}
 
void
String::Reverse()
{
   if ((_buffer)&&(HasChars()))
   {
      uint32 from = 0;
      uint32 to = Length()-1;
      while(from<to) muscleSwap(_buffer[from++], _buffer[to--]);
   }
}

uint32
String::Replace(char findChar, char replaceChar, uint32 maxReplaceCount)
{
   uint32 ret = 0; 
   if ((_buffer)&&(findChar != replaceChar))
   {
      char * c = _buffer;
      while((*c)&&(maxReplaceCount > 0))
      {
         if (*c == findChar) 
         {
            *c = replaceChar;
            maxReplaceCount--;
            ret++;
         }
         c++;
      }
   }
   return ret;
}
   
int32
String::Replace(const String & replaceMe, const String & withMe, uint32 maxReplaceCount)
{
   TCHECKPOINT;

   if (replaceMe.IsEmpty()) return -1;  // can't replace an empty string, that's silly!
   if (replaceMe == withMe) return muscleMin(maxReplaceCount, GetNumInstancesOf(replaceMe));  // no changes necessary!

   String temp;
   int32 perInstanceDelta = ((int32)withMe.Length())-((int32)replaceMe.Length());
   if (perInstanceDelta > 0)
   {
      // If we are replacing a shorter string with a longer string, we'll have to do a copy-and-swap
      uint32 numInstances = muscleMin(GetNumInstancesOf(replaceMe), maxReplaceCount);
      if (numInstances == 0) return 0;  // no changes necessary!
      if (temp.Prealloc(Length()+(perInstanceDelta*numInstances)) != B_NO_ERROR) return -1;
   }

   // This code works for both the in-place and the copy-over modes!
   int32 ret = 0;
   const char * readPtr = Cstr();
   char * writePtr = (perInstanceDelta > 0) ? temp._buffer : NULL;
   while(1)
   {
      char * nextReplaceMe = (maxReplaceCount>0) ? strstr((char *) readPtr, (char *) replaceMe()) : NULL;
      if (nextReplaceMe)
      {
         ret++;
         if (writePtr)
         {
            uint32 numBytes = (uint32) (nextReplaceMe-readPtr);
            if (perInstanceDelta != 0) memmove(writePtr, readPtr, numBytes);
            writePtr += numBytes;
         }
         else writePtr = nextReplaceMe;

         memcpy(writePtr, withMe(), withMe.Length());
         readPtr  = nextReplaceMe + replaceMe.Length();
         writePtr += withMe.Length();
         maxReplaceCount--;
      }
      else
      {
         if (writePtr)
         {
            // Finish up
            uint32 numBytes = (uint32) (Cstr()+Length()-readPtr);
            if (perInstanceDelta != 0) memmove(writePtr, readPtr, numBytes);
            writePtr += numBytes;
            *writePtr = '\0';
            if (perInstanceDelta > 0) 
            {
               temp._length = (uint32) (writePtr-temp());
               SwapContents(temp);
            }
            else _length = (uint32) (writePtr-Cstr());
         }
         return ret;
      }
   }
   return ret;  // just to shut the compiler up; we never actually get here
}

void
String::SwapContents(String & s)
{
   TCHECKPOINT;

   if (&s != this)
   {
      bool thisSmall = (  _buffer ==   _smallBuffer);
      bool sSmall    = (s._buffer == s._smallBuffer);

      if ((sSmall)&&(thisSmall))
      {
         for (int32 i=muscleMax(_length, s._length); i>=0; i--) muscleSwap(_smallBuffer[i], s._smallBuffer[i]);
      }
      else if (thisSmall)
      {
         _buffer      = s._buffer;
         _bufferLen   = s._bufferLen;
         s._buffer    = s._smallBuffer;
         s._bufferLen = sizeof(s._smallBuffer);
         memcpy(s._smallBuffer, _smallBuffer, _length+1);
      }
      else if (sSmall)
      {
         s._buffer    = _buffer;
         s._bufferLen = _bufferLen;
         _buffer      = _smallBuffer;
         _bufferLen   = sizeof(_smallBuffer);
         memcpy(_smallBuffer, s._smallBuffer, s._length+1);
      }
      else
      {
         muscleSwap(_buffer,    s._buffer);
         muscleSwap(_bufferLen, s._bufferLen);
      }

      muscleSwap(_length, s._length);   // always do this
   }
}

int
String::LastIndexOf(const String &s2, uint32 fromIndex) const
{
   TCHECKPOINT;

   if (s2.IsEmpty()) return Length()-1;
   if (fromIndex >= Length()) return -1;
   for (int i=fromIndex; i>=0; i--) if (strncmp(Cstr()+i, s2.Cstr(), s2.Length()) == 0) return i;
   return -1;
}

int
String::LastIndexOf(const char * s2, uint32 fromIndex) const
{
   TCHECKPOINT;

   if (s2 == NULL) s2 = "";
   uint32 s2Len = (uint32) strlen(s2);
   if (s2Len == 0) return Length()-1;
   if (fromIndex >= Length()) return -1;
   for (int i=fromIndex; i>=0; i--) if (strncmp(Cstr()+i, s2, s2Len) == 0) return i;
   return -1;
}

String
String::ToLowerCase() const
{
   String ret(*this);
   if (ret._buffer) for (uint32 i=0; i<ret.Length(); i++) ret._buffer[i] = (char)tolower(ret._buffer[i]);
   return ret;
}

String
String::ToUpperCase() const
{
   String ret(*this);
   if (ret._buffer) for (uint32 i=0; i<ret.Length(); i++) ret._buffer[i] = (char)toupper(ret._buffer[i]);
   return ret;
}
 
String 
String::Trim() const 
{ 
   TCHECKPOINT;

   int32 len = (int32) Length();
   const char * s = Cstr();
   int32 startIdx; for (startIdx = 0;     startIdx<len;    startIdx++) if (!IsSpaceChar(s[startIdx])) break; 
   int32 endIdx;   for (endIdx   = len-1; endIdx>startIdx; endIdx--)   if (!IsSpaceChar(s[endIdx]))   break; 
   return String(*this, (uint32)startIdx, (uint32)(endIdx+1));
}

uint32 String :: FlattenedSize() const
{
   return Length()+1;   
}

void String :: Flatten(uint8 *buffer) const
{
   memcpy((char *)buffer, Cstr(), Length()+1);
}

status_t String :: Unflatten(const uint8 *buf, uint32 size)
{
   return SetCstr((const char *)buf, size);
}

uint32 String :: GetNumInstancesOf(char ch) const
{
   uint32 ret = 0;
   for (const char * s = Cstr(); (*s != '\0'); s++) if (*s == ch) ret++; 
   return ret;
}

uint32 String :: GetNumInstancesOf(const String & substring) const
{
   TCHECKPOINT;

   uint32 ret = 0;
   if (substring.HasChars())
   {
      uint32 lastIdx = 0;
      int32 idx;
      while((idx = IndexOf(substring, lastIdx)) >= 0)
      {
         ret++;
         lastIdx = idx + substring.Length();
      }
   }
   return ret;
}

uint32 String :: GetNumInstancesOf(const char * substring) const
{
   TCHECKPOINT;

   if (substring == NULL) substring = "";
   uint32 ret = 0;
   uint32 substringLength = (uint32) strlen(substring);
   if (substringLength > 0)
   {
      uint32 lastIdx = 0;
      int32 idx;
      while((idx = IndexOf(substring, lastIdx)) >= 0)
      {
         ret++;
         lastIdx = idx + substringLength;
      }
   }
   return ret;
}

String String :: Prepend(const String & str, uint32 count) const
{
   TCHECKPOINT;

   String ret;
   uint32 newLen = (count*str.Length())+Length();
   if (ret.Prealloc(newLen) == B_NO_ERROR)
   {
      char * b = ret._buffer;

      if (str.HasChars())
      {
         for (uint32 i=0; i<count; i++)
         {
            memcpy(b, str(), str.Length());
            b += str.Length();
         }
      }
      if (HasChars())
      {
         memcpy(b, Cstr(), Length()); 
         b += Length();
      }
      ret._length = (uint32)(b-ret._buffer);
      ret._buffer[ret._length] = '\0';   // terminate the string
   }
   return ret;
}

String String :: Prepend(const char * str, uint32 count) const
{
   TCHECKPOINT;

   if (muscleInRange(str, (const char *)_buffer, (const char *)(_buffer+Length()))) return Prepend(String(str), count);  // avoid self-entanglement!
   else
   {
      if (str == NULL) str = "";
      uint32 sLen = (uint32) strlen(str);
      String ret;
      uint32 newLen = (count*sLen)+Length();
      if (ret.Prealloc(newLen) == B_NO_ERROR)
      {
         char * b = ret._buffer;

         if (sLen > 0)
         {
            for (uint32 i=0; i<count; i++)
            {
               memcpy(b, str, sLen);
               b += sLen;
            }
         }
         if (HasChars())
         {
            memcpy(b, Cstr(), Length()); 
            b += Length();
         }
         ret._length = (uint32)(b-ret._buffer);
         ret._buffer[ret._length] = '\0';   // terminate the string
      }
      return ret;
   }
}

String String :: Append(const String & str, uint32 count) const
{
   TCHECKPOINT;

   String ret;
   uint32 newLen = Length()+(count*str.Length());
   if (ret.Prealloc(newLen) == B_NO_ERROR)
   {
      char * b = ret._buffer;
      if (HasChars())
      {
         memcpy(b, Cstr(), Length()); 
         b += Length();
      }
      if (str.HasChars())
      {
         for (uint32 i=0; i<count; i++)
         {
            memcpy(b, str(), str.Length());
            b += str.Length();
         }
      }
      ret._length = (uint32)(b-ret._buffer);
      ret._buffer[ret._length] = '\0';   // terminate the string
   }
   return ret;
}

String String :: Append(const char * str, uint32 count) const
{
   TCHECKPOINT;

   if (muscleInRange(str, (const char *)_buffer, (const char *)(_buffer+Length()))) return Append(String(str), count);  // avoid self-entanglement!
   else
   {
      if (str == NULL) str = "";
      uint32 sLen = (uint32) strlen(str);
      String ret;
      uint32 newLen = Length()+(count*sLen);
      if (ret.Prealloc(newLen) == B_NO_ERROR)
      {
         char * b = ret._buffer;
         if (HasChars())
         {
            memcpy(b, Cstr(), Length()); 
            b += Length();
         }
         if (sLen > 0)
         {
            for (uint32 i=0; i<count; i++)
            {
               memcpy(b, str, sLen);
               b += sLen;
            }
         }
         ret._length = (uint32) (b-ret._buffer);
         ret._buffer[ret._length] = '\0';   // terminate the string
      }
      return ret;
   }
}

static uint32 GetNextBufferSize(uint32 bufLen)
{
   // For very small strings, we'll try to conserve memory by betting that they won't expand much more
   if (bufLen < 32) return bufLen+4;

   static const uint32 PAGE_SIZE = 4096;
   static const uint32 MALLOC_OVERHEAD = 12;  // we assume that malloc() might need as many as 12 bytes for book keeping

   // For medium-length strings, do a geometric expansion to reduce the number of reallocations
   if (bufLen < (PAGE_SIZE-MALLOC_OVERHEAD)) return bufLen*2;

   // For large (multi-page) allocations, we'll increase by one page.  According to Trolltech, modern implementations
   // of realloc() don't actually copy the entire large buffer, they just rearrange the memory map and add
   // a new page to the end, so this will be more efficient than it appears.
   uint32 curNumPages = (bufLen+MALLOC_OVERHEAD)/PAGE_SIZE; 
   return ((curNumPages+1)*PAGE_SIZE)-MALLOC_OVERHEAD;
}

// This method tries to ensure that at least (newBufLen) chars
// are available for storing data in.  (requestedBufLen) should include
// the terminating NUL.  If (retainValue) is true, the current string value
// will be retained; otherwise it should be set right after this call
// returns...
status_t String::EnsureBufferSize(uint32 requestedBufLen, bool retainValue)
{
   if (requestedBufLen > _bufferLen)
   {
      if (requestedBufLen <= sizeof(_smallBuffer))  // guaranteed only to be true the very first time we are called!
      {
         // For small initial requests, we can just set up our buffer pointer to point to our static-buffer area.  Cheap!
         _buffer    = _smallBuffer;
         _bufferLen = sizeof(_smallBuffer);
      }
      else
      {
         // If we're doing a first-time allocation, allocate exactly the number of the bytes requested.
         // If it's a re-allocation, allocate more than requested as it's more likely to happen yet another time...
         uint32 newBufLen = (_buffer == NULL) ? requestedBufLen : GetNextBufferSize(requestedBufLen);
         if (retainValue)
         {
            if ((_buffer)&&(_buffer != _smallBuffer))
            {
               // We can call muscleRealloc() to hopefully avoid data copying
               char * newBuf = (char *)muscleRealloc(_buffer, newBufLen);
               if (newBuf)
               {
                  _buffer    = newBuf;
                  _bufferLen = newBufLen;
               }
               else
               {
                  WARN_OUT_OF_MEMORY;
                  return B_ERROR;
               }
            }
            else
            {
               // Oops, muscleRealloc() won't do.... we'll just have to copy the bytes over
               char * newBuf = (char *) muscleAlloc(newBufLen);
               if (newBuf) *newBuf = '\0';  // ensure NUL termination
               else
               {
                  WARN_OUT_OF_MEMORY;
                  return B_ERROR;
               }
               if (_buffer) memcpy(newBuf, _buffer, Length()+1);
               if (_buffer != _smallBuffer) muscleFree(_buffer);
               _buffer    = newBuf;
               _bufferLen = newBufLen;
            }
         }
         else
         {
            // If the caller doesn't care about retaining the value, then it's
            // probably cheaper just to free our buffer and get a new one.
            char * newBuf = (char *) muscleAlloc(newBufLen);
            if (newBuf) *newBuf = '\0';  // ensure the new buffer is terminated
            else
            {
               WARN_OUT_OF_MEMORY;
               return B_ERROR;
            }
            if (_buffer != _smallBuffer) muscleFree(_buffer);
            _buffer    = newBuf;
            _bufferLen = newBufLen;
         }
      }
   }
   return B_NO_ERROR;
}

String String :: Pad(uint32 minLength, bool padOnRight, char padChar) const
{
   if (Length() < minLength)
   {
      uint32 padLen = minLength-Length();
      String temp; temp += padChar;
      return (padOnRight) ? Append(temp, padLen) : Prepend(temp, padLen);
   }
   else return *this;
}

#define ARG_IMPLEMENTATION   \
   char buf[256];            \
   sprintf(buf, fmt, value); \
   return ArgAux(buf)

String String :: Arg(int8   value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(uint8  value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(int16  value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(uint16 value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(int32  value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(uint32 value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(int64  value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(uint64 value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(double value, const char * fmt) const {ARG_IMPLEMENTATION;}
String String :: Arg(const String & value)           const {return ArgAux(value());}
String String :: Arg(const char * value)             const {return ArgAux(value);}

String String :: ArgAux(const char * buf) const
{
   TCHECKPOINT;

   int32 lowestArg = -1;
   const char * s = Cstr();
   while(*s != '\0')
   {
      if (*s == '%')
      {
         s++;
         if (muscleInRange(*s, '0', '9'))
         {
            int32 val = atol(s);
            lowestArg = (lowestArg < 0) ? val : muscleMin(val, lowestArg);
            while(muscleInRange(*s, '0', '9')) s++; 
         }
      }
      else s++;
   }

   if (lowestArg >= 0)
   {
      char token[64];
      sprintf(token, "%%"INT32_FORMAT_SPEC, lowestArg);
      String ret(*this);
      (void) ret.Replace(token, buf);
      return ret;
   }
   else return *this;
}

const String & GetEmptyString()
{
   static const String _empty;
   return _empty;
}

/* strnatcmp.c -- Perform 'natural order' comparisons of strings in C.
   Copyright (C) 2000, 2004 by Martin Pool <mbp sourcefrog net>

   This software is provided 'as-is', without any express or implied
   warranty.  In no event will the authors be held liable for any damages
   arising from the use of this software.
 
   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:
 
   1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original software.   (JAF-note:  I have altered
      this software slightly -- it is not the same as the original code!)
   3. This notice may not be removed or altered from any source distribution.
*/

/* These are defined as macros to make it easier to adapt this code to
 * different characters types or comparison functions. */
static inline int nat_isdigit(char a) {return isdigit((unsigned char) a);}
static inline int nat_isspace(char a) {return isspace((unsigned char) a);}
static inline char nat_toupper(char a) {return toupper((unsigned char) a);}

static int nat_compare_right(char const *a, char const *b)
{
   int bias = 0;
     
   /* The longest run of digits wins.  That aside, the greatest
      value wins, but we can't know that it will until we've scanned
      both numbers to know that they have the same magnitude, so we
      remember it in BIAS. */
   for (;; a++, b++) 
   {
           if (!nat_isdigit(*a)  &&  !nat_isdigit(*b)) return bias;
      else if (!nat_isdigit(*a)) return -1;
      else if (!nat_isdigit(*b)) return +1;
      else if (*a < *b) 
      {
         if (!bias) bias = -1;
      } 
      else if (*a > *b) 
      {
          if (!bias) bias = +1;
      } 
      else if ((!*a)&&(!*b)) return bias;
   }
   return 0;
}

static int nat_compare_left(char const *a, char const *b)
{
   /* Compare two left-aligned numbers: the first to have a different value wins. */
   while(1)
   {
          if ((!nat_isdigit(*a))&&(!nat_isdigit(*b))) return 0;
     else if (!nat_isdigit(*a)) return -1;
     else if (!nat_isdigit(*b)) return +1;
     else if (*a < *b)          return -1;
     else if (*a > *b)          return +1;
     ++a; ++b;
   }
   return 0;
}

static int strnatcmp0(char const *a, char const *b, int fold_case)
{
   int ai = 0;
   int bi = 0;
   while(1) 
   {
      char ca = a[ai];   
      char cb = b[bi];

      /* skip over leading spaces or zeros */
      while(nat_isspace(ca)) ca = a[++ai];
      while(nat_isspace(cb)) cb = b[++bi];
 
      /* process run of digits */
      if ((nat_isdigit(ca))&&(nat_isdigit(cb))) 
      {
         int fractional = (ca == '0' || cb == '0');
         if (fractional) 
         {
            int result = nat_compare_left(a+ai, b+bi);
            if (result != 0) return result;
         } 
         else 
         {
            int result = nat_compare_right(a+ai, b+bi);
            if (result != 0) return result;
         }
      }
 
      /* The strings compare the same.  Call strcmp() to break the tie. */
      if (!ca && !cb) return strcmp(a,b);
 
      if (fold_case) 
      {
         ca = nat_toupper(ca);
         cb = nat_toupper(cb);
      }
     
           if (ca < cb) return -1;
      else if (ca > cb) return +1;

      ++ai; ++bi;
   }
}

int strnatcmp(char const *a, char const *b)     {return strnatcmp0(a, b, 0);}
int strnatcasecmp(char const *a, char const *b) {return strnatcmp0(a, b, 1);}

int NumericAwareStrcmp(const char * s1, const char * s2)     {return strnatcmp(    s1, s2);}
int NumericAwareStrcasecmp(const char * s1, const char * s2) {return strnatcasecmp(s1, s2);}

}; // end namespace muscle