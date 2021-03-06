// "interface_call" -*- C++ -*-

// Copyright (C) 2016 Aaron Conole
//
// This file is governed by the 'use this freely' common-sense license
// This means the following:
// - Take this header
// - #include it in your project (commercial, or non)
// - ???
// - profit


#pragma once

#include <iostream>

#include "property_bag.hpp"

#ifndef __INTERFACE_CALL__H__
#define __INTERFACE_CALL__H__

namespace cxx_utils
{
    namespace misc
    {

        /**
         * @brief Interface-call represents any kind of remote procedure
         * invocation mechanism (cli commands, SOAP, XML/RPC, etc.)
         *
         * The interface_call is a callback mechanism. 
         */
        class interface_call
        {
        public:
            interface_call(){}
            virtual ~interface_call(){}
            
            virtual const char *format() const = 0;
            virtual int32_t help(std::ostream &rOutput, const size_t token) = 0;
            virtual int32_t invoke(std::ostream &rOutput, std::istream &rInput, cxx_utils::misc::property_bag &rParameters) = 0;
        };
    }
}
