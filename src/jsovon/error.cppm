export module jsovon.error;

import std;

export namespace jsovon {

class ErrorBase : public std::exception {
public:
    ErrorBase(std::string msg) { this->msg = msg; }
    ErrorBase(const char msg[]) { this->msg = std::string(msg); }
    ~ErrorBase() {};
    const char *what() const noexcept override { return this->msg.c_str(); }

private:
    std::string msg;
};

/**
 * Parser Error
 */

class ReadFileError : public ErrorBase {
public:
    ReadFileError(const char msg[])  : ErrorBase(msg) { }
    ReadFileError(std::string msg)  : ErrorBase(msg) { }
};

class ParseError : public ErrorBase {
public:
    ParseError(const char msg[])  : ErrorBase(msg) { }
    ParseError(std::string msg)  : ErrorBase(msg) { }
};

/**
 * JSON Error
 */

class KeyNotFoundError : public ErrorBase {
public:
    KeyNotFoundError(const char msg[])  : ErrorBase(msg) { }
    KeyNotFoundError(std::string msg)  : ErrorBase(msg) { }
};

class DuplicatedKetError : public ErrorBase {
public:
    DuplicatedKetError(const char msg[])  : ErrorBase(msg) { }
    DuplicatedKetError(std::string msg)  : ErrorBase(msg) { }
};

class OperationNotAllowError : public ErrorBase {
public:
    OperationNotAllowError(const char msg[])  : ErrorBase(msg) { }
    OperationNotAllowError(std::string msg)  : ErrorBase(msg) { }
};

class IndexOutOfRangeError : public ErrorBase {
public:
    IndexOutOfRangeError(const char msg[])  : ErrorBase(msg) { }
    IndexOutOfRangeError(std::string msg)  : ErrorBase(msg) { }
};

class InternalError : public ErrorBase {
public:
    InternalError(const char msg[])  : ErrorBase(msg) { }
    InternalError(std::string msg)  : ErrorBase(msg) { }
};

class TypeNotMatchError : public ErrorBase {
public:
    TypeNotMatchError(const char msg[])  : ErrorBase(msg) { }
    TypeNotMatchError(std::string msg)  : ErrorBase(msg) { }
};

};
