/* 
 * The Biomechanical ToolKit
 * Copyright (c) 2009-2015, Arnaud Barré
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __btkAny_cast_tpp
#define __btkAny_cast_tpp

#include "btkException.h"
#include "btkTypeTraits.h"

#include <string>

#include <cstdlib> // strtol, strtoll, strtoul, strtoull, strtof, strtod, ...

namespace btk
{
  struct Any::details
  {
    details() = delete;
    ~details() noexcept = delete;
    details(const details& ) = delete;
    details(details&& ) noexcept = delete;
    details& operator=(const details& ) = delete;
    details& operator=(details&& ) noexcept = delete;
    
    // Default
    template <typename U>
    static inline typename std::enable_if<
          !std::is_arithmetic<typename std::decay<U>::type>::value
       && !std::is_same<std::string, typename std::decay<U>::type>::value
       && !is_stl_vector<typename std::decay<U>::type>::value
      , bool>::type cast(U* , StorageBase* , size_t = 0) noexcept
    {
      return false;
    };
    
    // Arithmetic conversion
    template <typename U>
    static typename std::enable_if<std::is_arithmetic<typename std::decay<U>::type>::value, bool>::type cast(U* value, StorageBase* storage, size_t idx = 0) noexcept
    {
      const typeid_t id = storage->id();
      if (storage->is_arithmetic())
      {
        // bool
        if (id == static_typeid<bool>())
          *value = static_cast<U>(static_cast<bool*>(storage->Data)[idx]);
        // char
        else if (id == static_typeid<char>())
          *value = static_cast<U>(static_cast<char*>(storage->Data)[idx]);
        // char16_t
        else if (id == static_typeid<char16_t>())
          *value = static_cast<U>(static_cast<char16_t*>(storage->Data)[idx]);
        // char32_t
        else if (id == static_typeid<char32_t>())
          *value = static_cast<U>(static_cast<char32_t*>(storage->Data)[idx]);
        // wchar_t
        else if (id == static_typeid<wchar_t>())
          *value = static_cast<U>(static_cast<wchar_t*>(storage->Data)[idx]);
        // signed char
        else if (id == static_typeid<signed char>())
          *value = static_cast<U>(static_cast<signed char*>(storage->Data)[idx]);
        // short int
        else if (id == static_typeid<short int>())
          *value = static_cast<U>(static_cast<short int*>(storage->Data)[idx]);
        // int
        else if (id == static_typeid<int>())
          *value = static_cast<U>(static_cast<int*>(storage->Data)[idx]);
        // long int
        else if (id == static_typeid<long int>())
          *value = static_cast<U>(static_cast<long int*>(storage->Data)[idx]);
        // long long int
        else if (id == static_typeid<long long int>())
          *value = static_cast<U>(static_cast<long long int*>(storage->Data)[idx]);
        // unsigned char
        else if (id == static_typeid<unsigned char>())
          *value = static_cast<U>(static_cast<unsigned char*>(storage->Data)[idx]);
        // unsigned short int
        else if (id == static_typeid<unsigned short int>())
          *value = static_cast<U>(static_cast<unsigned short int*>(storage->Data)[idx]);
        // unsigned int
        else if (id == static_typeid<unsigned int>())
          *value = static_cast<U>(static_cast<unsigned int*>(storage->Data)[idx]);
        // unsigned long int
        else if (id == static_typeid<unsigned long int>())
          *value = static_cast<U>(static_cast<unsigned long int*>(storage->Data)[idx]);
        // unsigned long long int
        else if (id == static_typeid<unsigned long long int>())
          *value = static_cast<U>(static_cast<unsigned long long int*>(storage->Data)[idx]);
        // float
        else if (id == static_typeid<float>())
          *value = static_cast<U>(static_cast<float*>(storage->Data)[idx]);
        // double
        else if (id == static_typeid<double>())
          *value = static_cast<U>(static_cast<double*>(storage->Data)[idx]);
        // long double
        else if (id == static_typeid<long double>())
          *value = static_cast<U>(static_cast<long double*>(storage->Data)[idx]);
        // ERROR - Should not be possible! All the standard arithmetic type in C++11 are listed above
        else
          throw(LogicError("Unexpected error during arithmetic to arithmetic conversion!"));
        return true;
      }
      else if (id == static_typeid<std::string>())
      {
        const char* str = static_cast<std::string*>(storage->Data)[idx].c_str();
        cast_from_string(value,str);
        return true;
      }
      return false;
    };
    
    // String conversion
    template <typename U>
    static typename std::enable_if<std::is_same<std::string, typename std::decay<U>::type>::value, bool>::type cast(U* value, StorageBase* storage, size_t idx = 0) noexcept
    {
      const typeid_t id = storage->id();
      if (storage->is_arithmetic()
               && (id != static_typeid<char16_t>())
               && (id != static_typeid<char32_t>())
               && (id != static_typeid<wchar_t>()))
      {
        // bool
        if (id == static_typeid<bool>())
          *value = std::string(static_cast<bool*>(storage->Data)[idx] ? "true" : "false");
        // char (convert as it is an int8_t)
        else if (id == static_typeid<char>())
          *value = std::to_string((short int)static_cast<char*>(storage->Data)[idx]);
        // signed char (convert as it is a signed int8_t)
        else if (id == static_typeid<signed char>())
          *value = std::to_string((signed short int)static_cast<signed char*>(storage->Data)[idx]);
        // short int
        else if (id == static_typeid<short int>())
          *value = std::to_string(static_cast<short int*>(storage->Data)[idx]);
        // int
        else if (id == static_typeid<int>())
          *value = std::to_string(static_cast<int*>(storage->Data)[idx]);
        // long int
        else if (id == static_typeid<long int>())
          *value = std::to_string(static_cast<long int*>(storage->Data)[idx]);
        // long long int
        else if (id == static_typeid<long long int>())
          *value = std::to_string(static_cast<long long int*>(storage->Data)[idx]);
        // unsigned char (convert as it is a unsigned int8_t)
        else if (id == static_typeid<unsigned char>())
          *value = std::to_string((unsigned short int)static_cast<unsigned char*>(storage->Data)[idx]);
        // unsigned short int
        else if (id == static_typeid<unsigned short int>())
          *value = std::to_string(static_cast<unsigned short int*>(storage->Data)[idx]);
        // unsigned int
        else if (id == static_typeid<unsigned int>())
          *value = std::to_string(static_cast<unsigned int*>(storage->Data)[idx]);
        // unsigned long int
        else if (id == static_typeid<unsigned long int>())
          *value = std::to_string(static_cast<unsigned long int*>(storage->Data)[idx]);
        // unsigned long long int
        else if (id == static_typeid<unsigned long long int>())
          *value = std::to_string(static_cast<unsigned long long int*>(storage->Data)[idx]);
        // float
        else if (id == static_typeid<float>())
          *value = std::to_string(static_cast<float*>(storage->Data)[idx]);
        // double
        else if (id == static_typeid<double>())
          *value = std::to_string(static_cast<double*>(storage->Data)[idx]);
        // long double
        else if (id == static_typeid<long double>())
          *value = std::to_string(static_cast<long double*>(storage->Data)[idx]);
        // ERROR - Should not be possible! All the standard arithmetic type in C++11 are listed above
        else
          throw(LogicError("Unexpected error during arithmetic to string conversion!"));
        return true;
      }
      return false;
    };
    
    // Vector conversion
    template <typename U>
    static typename std::enable_if<is_stl_vector<typename std::decay<U>::type>::value, bool>::type cast(U* value, StorageBase* storage) noexcept
    {
      value->resize(storage->size());
      for (size_t i = 0 ; i < value->size() ; ++i)
        cast(&value->operator[](i),storage,i);
      return true;
    };
  
    // --------------------------------------------------------------------- //
    
  private:
    
    template <typename U>
    static inline typename std::enable_if<std::is_same<bool, typename std::decay<U>::type>::value>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = (!((strlen(str) == 0) || (strcmp(str,"0") == 0) || (strcmp(str,"false") == 0)));
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_integral<typename std::decay<U>::type>::value && !std::is_same<bool, typename std::decay<U>::type>::value && std::is_signed<typename std::decay<U>::type>::value && (sizeof(typename std::decay<U>::type) > sizeof(long))>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtoll(str,nullptr,0);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_integral<typename std::decay<U>::type>::value && !std::is_same<bool, typename std::decay<U>::type>::value && std::is_signed<typename std::decay<U>::type>::value && (sizeof(typename std::decay<U>::type) <= sizeof(long))>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtol(str,nullptr,0);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_integral<typename std::decay<U>::type>::value && !std::is_same<bool, typename std::decay<U>::type>::value && std::is_unsigned<typename std::decay<U>::type>::value && (sizeof(typename std::decay<U>::type) > sizeof(long))>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtoull(str,nullptr,0);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_integral<typename std::decay<U>::type>::value && !std::is_same<bool, typename std::decay<U>::type>::value && std::is_unsigned<typename std::decay<U>::type>::value && (sizeof(typename std::decay<U>::type) <= sizeof(long))>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtol(str,nullptr,0);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_same<float, typename std::decay<U>::type>::value>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtof(str,nullptr);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_same<double, typename std::decay<U>::type>::value>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtod(str,nullptr);
    };
  
    template <typename U>
    static inline typename std::enable_if<std::is_same<long double, typename std::decay<U>::type>::value>::type cast_from_string(U* value, const char* str) noexcept
    {
      *value = strtold(str,nullptr);
    };
  };
};

#endif // __btkAny_cast_tpp