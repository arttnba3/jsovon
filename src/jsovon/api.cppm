export module jsovon.api;

import jsovon.parser;
import jsovon.wrapper;

export namespace jsovon {

template<typename T>
auto ParseJsonFile(T file) -> Json
{
    JsonParser parser;

    parser.SetInputFile(file);

    return parser.Parse();
}

template<typename T>
auto ParseJsonText(T text) -> Json
{
    JsonParser parser;

    parser.SetInputText(text);

    return parser.Parse();
}

};
