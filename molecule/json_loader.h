/****************************************************************************
 * Copyright (C) from 2009 to Present EPAM Systems.
 *
 * This file is part of Indigo toolkit.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ***************************************************************************/

#ifndef json_loader_hpp
#define json_loader_hpp

#include "base_cpp/properties_map.h"
#include "base_cpp/red_black.h"
#include "base_cpp/tlscont.h"
#include "third_party/rapidjson/document.h"

using namespace rapidjson;
namespace indigo
{
    class Scanner;
    class JSONLoader
    {
    public:
        JSONLoader( Scanner& scanner );
        ~JSONLoader();
        
        bool hasNext();
        const Value& next();
        const Value& at( int index );
        
        int currentNumber();
        int count();
        
        void readAt( int index );
        CP_DECL;
        DECL_ERROR;
        
    protected:
        Scanner* _scanner;
        int _current_number;
        Document _data;
        const Value* _nodes;
    };
    
} // namespace indigo

#endif /* json_loader_hpp */
