export module jsovon.parser;

import jsovon.error;
import jsovon.scanner;
import jsovon.wrapper;
import std;

namespace jsovon {

export class JsonParser {
public:
    JsonParser(void);
    ~JsonParser();

    template<typename T>
    auto SetInputFile(T input) -> void;

    template<typename T>
    auto SetInputText(T input) -> void;

    auto Parse(void) -> Json;

private:
    JsonScanner scanner;

    auto ParseObject(void) -> Json;
    auto ParseArray(void) -> Json;

    auto NumStrParsing(std::string lexeme) -> Json;
};

JsonParser::JsonParser(void)
{
    /* do nothing? */
}

JsonParser::~JsonParser(void)
{
    /* do nothing? */
}

template<typename T>
auto JsonParser::SetInputFile(T input) -> void
{
    this->scanner.SetInputFile(input);
}

template<typename T>
auto JsonParser::SetInputText(T input) -> void
{
    this->scanner.SetInputText(input);
}

auto JsonParser::Parse(void) -> Json
{
    Token token;
    Json json;

    token = this->scanner.GetNextToken();

    switch (token.type) {
    case TOKEN_LBRACE:
        this->scanner.PushToken(token);
        return this->ParseObject();
    case TOKEN_LBRACKET:
        this->scanner.PushToken(token);
        return this->ParseArray();
    default:
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
        break;
    }

    return json;
}

static std::unordered_map<std::string, std::size_t> json_object_initer;

/**
 * OBJECT -> L_BRACE OBJ_ELEMENTS R_BRACE
 * OBJ_ELEMENTS -> NONE | OBJ_ELEMENT | OBJ_ELEMENT COMMA OBJ_ELEMENT
 * OBJ_ELEMENT -> STRING COLON JSON_ELEMENT
 */
auto JsonParser::ParseObject(void) -> Json
{
    Json res(json_object_initer);
    Token token;
    std::string key;
    Json val;

    token = this->scanner.GetNextToken();
    if (token.type != TOKEN_LBRACE) {
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
    }

    token = this->scanner.GetNextToken();
    if (token.type == TOKEN_RBRACE) {   /* for the first element only */
        goto out;
    }

obj_elem_parse_start:
    switch (token.type) {
    case TOKEN_STRING:
        key = token.lexeme;
        break;
    default:
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
    }

    token = this->scanner.GetNextToken();
    if (token.type != TOKEN_COLON) {
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
    }

    token = this->scanner.GetNextToken();
    switch (token.type) {
    case TOKEN_STRING:
        val = token.lexeme;
        break;
    case TOKEN_NUMBER:
        val = this->NumStrParsing(token.lexeme);
        break;
    case TOKEN_BOOL:
        val = (token.lexeme == "true") ? true : false;
        break;
    case TOKEN_NULL:
        val.BeNull();
        break;
    case TOKEN_LBRACE:
        this->scanner.PushToken(token);
        val = this->ParseObject();
        break;
    case TOKEN_LBRACKET:
        this->scanner.PushToken(token);
        val = this->ParseArray();
        break;
    default:
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
    }

    res[key] = val;

    /* again or done */
    token = this->scanner.GetNextToken();
    if (token.type == TOKEN_COMMA) {
        token = this->scanner.GetNextToken();
        goto obj_elem_parse_start;
    } else if (token.type != TOKEN_RBRACE) {
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
    }

out:
    return res;
}

static std::vector<std::size_t> json_array_initer;

/**
 * ARRAY -> L_BRACKET ARR_ELEMENTS R_BRACKET
 * ARR_ELEMENTS -> NONE | ARR_ELEMENT | ARR_ELEMENT COMMA ARR_ELEMENT
 * ARR_ELEMENT -> JSON_ELEMENT
 */
auto JsonParser::ParseArray(void) -> Json
{
    Json res(json_array_initer);
    Token token;
    Json val;

    token = this->scanner.GetNextToken();
    if (token.type != TOKEN_LBRACKET) {
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
    }

    token = this->scanner.GetNextToken();
    if (token.type == TOKEN_RBRACKET) { /* for the first element only */
        goto out;
    }

arr_elem_parse_start:
    switch (token.type) {
    case TOKEN_STRING:
        val = token.lexeme;
        break;
    case TOKEN_NUMBER:
        val = this->NumStrParsing(token.lexeme);
        break;
    case TOKEN_BOOL:
        val = (token.lexeme == "true") ? true : false;
        break;
    case TOKEN_NULL:
        val.BeNull();
        break;
    case TOKEN_LBRACE:
        this->scanner.PushToken(token);
        val = this->ParseObject();
        break;
    case TOKEN_LBRACKET:
        this->scanner.PushToken(token);
        val = this->ParseArray();
        break;
    default:
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
    }

    res.append(val);

    /* again or done */
    token = this->scanner.GetNextToken();
    if (token.type == TOKEN_COMMA) {
        token = this->scanner.GetNextToken();
        goto arr_elem_parse_start;
    } else if (token.type != TOKEN_RBRACKET) {
        throw ParseError(
            std::format("Unexpected ident \"{}\" at line {}, column {}", token.orig_str, token.line, token.off)
        );
    }

out:
    return res;
}

/* Note that we assume the token's lexeme is ALWAYS VALID here */
auto JsonParser::NumStrParsing(std::string lexeme) -> Json
{
    Json res;
    std::int64_t ival = 0, exponent = 0, expo_ival = 10;
    double fval = 0, frac_bit = 0.1, expo_fval = 10.0;
    bool has_fraction = false, negative_exponent = false;
    std::size_t idx;

    for (idx = 0; idx < lexeme.size(); idx++) {
        if (lexeme[idx] == '.') {
            idx++;
            has_fraction = true;
            break;
        } else if (lexeme[idx] == 'e' || lexeme[idx] == 'E') {
            idx++;
            break;
        }

        ival *= 10;
        ival += lexeme[idx] - '0';
    }

    if (!has_fraction) {
        goto parse_exponent;
    }

    fval = ival;
    while (idx < lexeme.size()) {
        if (lexeme[idx] == 'e' || lexeme[idx] == 'E') {
            idx++;
            break;
        }

        fval += frac_bit * static_cast<double>(lexeme[idx] - '0');
        frac_bit *= 0.1;
        idx++;
    }

    if (idx == lexeme.size()) {
        goto out;
    }

parse_exponent:
    if (lexeme[idx] == '-') {
        negative_exponent = true;
        idx++;
    } else if (lexeme[idx] == '+') {
        idx++;
    }

    while (idx < lexeme.size()) {
        exponent *= 10;
        exponent += lexeme[idx++] - '0';
    }

out:
    if (!has_fraction) {
        if (!negative_exponent) {
            while (exponent > 0) {
                if (exponent & 1) {
                    ival *= expo_ival;
                }

                expo_ival *= expo_ival;
                exponent >>= 1;
            }
        } else {
            while (exponent > 0) {
                if (exponent & 1) {
                    ival /= expo_ival;
                }

                expo_ival *= expo_ival;
                exponent >>= 1;
            }
        }

        res = ival;
    } else {
        if (!negative_exponent) {
            while (exponent > 0) {
                if (exponent & 1) {
                    fval *= expo_fval;
                }

                expo_fval *= expo_fval;
                exponent >>= 1;
            }
        } else {
            while (exponent > 0) {
                if (exponent & 1) {
                    fval /= expo_fval;
                }

                expo_fval *= expo_fval;
                exponent >>= 1;
            }
        }

        res = fval;
    }

    return res;
}

};
