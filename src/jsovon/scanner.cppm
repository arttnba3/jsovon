export module jsovon.scanner;

import jsovon.error;
import std;

export namespace jsovon {

enum TokenType {
    /* values */
    TOKEN_STRING = 0, TOKEN_NUMBER, TOKEN_BOOL, TOKEN_NULL,
    /* seperator , : [] {} */
    TOKEN_COMMA, TOKEN_COLON, TOKEN_LBRACKET, TOKEN_RBRACKET, TOKEN_LBRACE, TOKEN_RBRACE,
    /* operator + -  */
    TOKEN_PLUS, TOKEN_MINUS, 
    /* error token */
    TOKEN_ERR,
    /* end of token flow */
    TOKEN_END,
    /* num of types */
    TOKEN_TYPES,
};

std::string token_type_str[TOKEN_TYPES] = {
    "String", "Number", "Bool", "Null",
    "Comma", "Colon", "LBracket", "RBracket", "LBrace", "RBrace",
    "Plus", "Minus",
    "Err", "End",
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::string orig_str;   /* for debug only */

    /* for error report only */

    std::size_t line;
    std::size_t off;

    /* for test only */
    friend std::ostream &operator<<(std::ostream &output, const Token &token)
    {
        switch (token.type) {
        case TOKEN_STRING:
            output << std::format("Token(String({}))", token.lexeme);
            break;
        case TOKEN_NUMBER:
            output << std::format("Token(Number({}))", token.lexeme);
            break;
        case TOKEN_BOOL:
            output << std::format("Token(Bool({}))", token.lexeme);
            break;
        default:
            output << std::format("Token({})", token_type_str[token.type]);
            break;
        }
        return output;
    }
};

#define SCANNER_BUF_SZ 0x1000

#ifndef EOF
    #define EOF (-1)
#endif

class JsonScanner {
public:
    JsonScanner();
    ~JsonScanner();

    auto GetNextToken(void) -> Token;
    template <typename T>
    auto PushToken(T token) -> void;

    template<typename T>
    auto SetInputFile(T input) -> void;

    template<typename T>
    auto SetInputText(T input) -> void;

private:
    char buf[SCANNER_BUF_SZ];
    std::size_t start = 0, end = 0;
    std::size_t lineno = 1, lineoff = 0, last_lineoff;
    std::istream *input;

    auto pop_char(void) -> char;
    auto push_char(char ch) -> void;

    auto GetNextTokenString(void) -> Token;
    auto GetNextTokenNumber(void) -> Token;
    auto GetNextTokenBool(void) -> Token;
    auto GetNextTokenNull(void) -> Token;

    std::queue<Token> token_queue;
};

JsonScanner::JsonScanner()
{
    this->input = nullptr;
}

JsonScanner::~JsonScanner()
{
    if (this->input) {
        delete this->input;
    }
}

template <typename T>
auto JsonScanner::PushToken(T token) -> void
{
    this->token_queue.push(token);
}

auto JsonScanner::GetNextToken(void) -> Token
{
    Token token;
    char ch;
    std::size_t lineno, lineoff;

    if (!this->token_queue.empty()) {
        token = this->token_queue.front();
        this->token_queue.pop();
        return token;
    }

    /* clear space */
    do {
        ch = this->pop_char();
    } while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t');

    if (ch == EOF) {
        token.type = TOKEN_END;
        return token;
    }

    /* pre-save for token-start loc */
    lineno = this->lineno;
    lineoff = this->lineoff;

    /* only for single-char token */
    token.lexeme = ch;
    token.orig_str = ch;

    switch (ch) {
    case '"':
        token = this->GetNextTokenString();
        break;
    case '{':
        token.type = TOKEN_LBRACE;
        break;
    case '}':
        token.type = TOKEN_RBRACE;
        break;
    case '[':
        token.type = TOKEN_LBRACKET;
        break;
    case ']':
        token.type = TOKEN_RBRACKET;
        break;
    case ',':
        token.type = TOKEN_COMMA;
        break;
    case ':':
        token.type = TOKEN_COLON;
        break;
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        this->push_char(ch);
        token = this->GetNextTokenNumber();
        break;
    case 't':
    case 'f':
        this->push_char(ch);
        token = this->GetNextTokenBool();
        break;
    case 'n':
        this->push_char(ch);
        token = this->GetNextTokenNull();
        break;
    default:
        token.type = TOKEN_ERR;
        break;
    }

    token.line = lineno;
    token.off = lineoff;

    return token;
}

auto JsonScanner::GetNextTokenString(void) -> Token
{
    Token token;
    char ch, uch;

    token.orig_str = "\"";

    while (true) {
        ch = this->pop_char();
        token.orig_str += ch;
        if (ch == '\\') {
            ch = this->pop_char();
            token.orig_str += ch;
            switch (ch) {
            case '"':
                token.lexeme += '"';
                break;
            case '\\':
                token.lexeme += '\\';
                break;
            case '/':
                token.lexeme += '/';
                break;
            case 'b':
                token.lexeme += '\b';
                break;
            case 'f':
                token.lexeme += '\f';
                break;
            case 'n':
                token.lexeme += '\n';
                break;
            case 'r':
                token.lexeme += '\r';
                break;
            case 't':
                token.lexeme += '\t';
                break;
            case 'u':
                for (auto i = 0; i < 4; i++) {
                    uch = this->pop_char();
                    if (uch == EOF) {
                        token.type = TOKEN_ERR;
                        return token;
                    }

                    token.orig_str += uch;

                    if (i % 2 == 0) {
                        ch = '\0';
                    }

                    ch <<= 4;
                    if (uch >= '0' && uch <= '9') {
                        ch += (uch - '0');
                    } else if (uch >= 'A' && uch <= 'F') {
                        ch += (uch - 'A') + 10;
                    } else if (uch >= 'a' && uch <= 'f') {
                        ch += (uch - 'a') + 10;
                    } else {
                        token.type = TOKEN_ERR;
                        return token;
                    }

                    if (i % 2 != 0) {
                        token.lexeme += ch;
                    }
                }
                break;
            default:
                token.type = TOKEN_ERR;
                return token;
            }
        } else if (ch == EOF) {
            token.type = TOKEN_ERR;
            return token;
        } else {
            if (ch == '"') {
                break;
            }
            token.lexeme += ch;
        }
    }

    token.type = TOKEN_STRING;

    return token;
}

auto JsonScanner::GetNextTokenNumber(void) -> Token
{
    Token token;
    char ch;

    ch = this->pop_char();
    if (ch == '-') {
        token.lexeme += ch;
        token.orig_str += ch;
    } else {
        this->push_char(ch);
    }

    ch = this->pop_char();
    if (ch == '0') {
        token.lexeme += ch;
        token.orig_str += ch;
        goto fraction;
    } else {
        this->push_char(ch);
    }

    ch = this->pop_char();
    token.orig_str += ch;
    if (ch >= '1' && ch <= '9') {
        token.lexeme += ch;
    } else {
        token.type = TOKEN_ERR;
        return token;
    }

    while (true) {
        ch = this->pop_char();
        if (ch >= '0' && ch <= '9') {
            token.lexeme += ch;
            token.orig_str += ch;
        } else {
            this->push_char(ch);
            break;
        }
    }

fraction:
    ch = this->pop_char();
    if (ch == '.') {
        token.lexeme += ch;
        token.orig_str += ch;

        while (true) {
            ch = this->pop_char();
            if (ch >= '0' && ch <= '9') {
                token.lexeme += ch;
                token.orig_str += ch;
            } else {
                this->push_char(ch);
                break;
            }
        }
    } else {
        this->push_char(ch);
    }

    /* exponent */

    ch = this->pop_char();
    if (ch == 'e' || ch == 'E') {
        token.lexeme += ch;
        token.orig_str += ch;

        ch = this->pop_char();
        if (ch == '+' || ch == '-') {
            token.lexeme += ch;
            token.orig_str += ch;
        } else {
            this->push_char(ch);
        }
    } else {
        this->push_char(ch);
        goto end;
    }

    ch = this->pop_char();
    if (ch >= '0' && ch <= '9') {
        token.lexeme += ch;
        token.orig_str += ch;
    } else {
        token.type = TOKEN_ERR;
        return token;
    }

    while (true) {
        ch = this->pop_char();
        if (ch >= '0' && ch <= '9') {
            token.lexeme += ch;
            token.orig_str += ch;
        } else {
            this->push_char(ch);
            break;
        }
    }

end:
    ch = this->pop_char();
    switch (ch) {
    case '{':
    case '}':
    case '[':
    case ']':
    case ',':
        this->push_char(ch);
    case ' ':
    case '\n':
    case '\r':
    case '\t':
        break;
    default:
        token.type = TOKEN_ERR;
    }

    token.type = TOKEN_NUMBER;

    return token;
}

auto JsonScanner::GetNextTokenBool(void) -> Token
{
    Token token;
    char ch;

    token.type = TOKEN_BOOL;

    ch = this->pop_char();
    token.lexeme += ch;
    token.orig_str += ch;
    if (ch == 't') {
        for (auto i = 1; i < 4; i++) {
            ch = this->pop_char();
            token.lexeme += ch;
            token.orig_str += ch;

            if (ch != "true"[i]) {
                token.type = TOKEN_ERR;
                return token;
            }
        }
    } else if (ch == 'f') {
        for (auto i = 1; i < 5; i++) {
            ch = this->pop_char();
            token.lexeme += ch;
            token.orig_str += ch;

            if (ch != "false"[i]) {
                token.type = TOKEN_ERR;
                return token;
            }
        }
    } else {
        throw InternalError("JsonScanner input stream corrupted.");
    }

    ch = this->pop_char();
    switch (ch) {
    case '{':
    case '}':
    case '[':
    case ']':
    case ',':
        this->push_char(ch);
    case ' ':
    case '\n':
    case '\r':
    case '\t':
        break;
    default:
        token.type = TOKEN_ERR;
    }

    return token;
}

auto JsonScanner::GetNextTokenNull(void) -> Token
{
        Token token;
    char ch;

    token.type = TOKEN_NULL;

    ch = this->pop_char();
    token.lexeme += ch;
    token.orig_str += ch;
    if (ch == 'n') {
        for (auto i = 1; i < 4; i++) {
            ch = this->pop_char();
            token.lexeme += ch;
            token.orig_str += ch;

            if (ch != "null"[i]) {
                token.type = TOKEN_ERR;
                return token;
            }
        }
    } else {
        throw InternalError("JsonScanner input stream corrupted.");
    }

    ch = this->pop_char();
    switch (ch) {
    case '{':
    case '}':
    case '[':
    case ']':
    case ',':
        this->push_char(ch);
    case ' ':
    case '\n':
    case '\r':
    case '\t':
        break;
    default:
        token.type = TOKEN_ERR;
    }

    return token;
}

template<typename T>
auto JsonScanner::SetInputFile(T input) -> void
{
    if (this->input) {
        delete this->input;
        this->lineno = 0;
        this->lineoff = 0;
        this->last_lineoff = 0;
        this->start = 0;
        this->end = 0;
    }

    this->input = new std::ifstream(input, std::ios::in);
}

template<typename T>
auto JsonScanner::SetInputText(T input) -> void
{
    if (this->input) {
        delete this->input;
        this->lineno = 0;
        this->lineoff = 0;
        this->last_lineoff = 0;
        this->start = 0;
        this->end = 0;
    }

    this->input = new std::istringstream(input);
}

auto JsonScanner::pop_char(void) -> char
{
    std::size_t left;
    std::streamsize done;
    char ch;

    if (start < end) {
        goto out;
    }

    if (this->input->eof()) {
        return EOF;
    }

    if (!this->input || !(*this->input)) {
        throw ReadFileError("Failed to open input file or not configure!");
    }

    if (this->start == SCANNER_BUF_SZ) {
        this->start = 0;
    }

    left = SCANNER_BUF_SZ - this->start;

    this->input->read(&this->buf[this->start], left);
    done = this->input->gcount();

    if (done <= 0) {
        return EOF;
    }

    this->end = this->start + done;

out:
    ch = this->buf[this->start++];
    this->lineoff++;
    if (ch == '\n') {
        this->lineno++;
        this->last_lineoff = this->lineoff;
        this->lineoff = 0;
    }
    return ch;
}

auto JsonScanner::push_char(char ch) -> void
{
    if (this->start == 0) {
        std::size_t len;

        if (this->end == SCANNER_BUF_SZ) {
            return ;
        }

        len = this->end - this->start;
        for (auto i = 0; i < len; i++) {
            this->buf[this->end - i] = this->buf[this->end - i - 1];
        }

        this->start++;
        this->end++;
    }

    this->buf[--this->start] = ch;
    if (ch == '\n') {
        this->lineno--;
        this->lineoff = this->last_lineoff;
    } else if (this->lineoff > 0) {
        this->lineoff--;
    }
}

}
