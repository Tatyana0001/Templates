#include <iostream>
#include <mutex>
#include <fstream>

class LogMessage {
public:
    enum class Type {
        FatalError,
        Error,
        Warning,
        Unknown,
    };
    virtual Type type() const = 0;
    virtual const std::string& message() const = 0;
};

class LogHandler {
public:
    explicit LogHandler(std::unique_ptr <LogHandler> next) : next_(std::move(next)) {}
    void receive(const LogMessage& msg) {
        if (HandlePush(msg)) {
            return;
        }
        if (!next_) {
            throw std::runtime_error("Error: Message should be handled!");
        }
        next_->receive(msg);
    }
private:
    virtual bool HandlePush(const LogMessage& msg) = 0;
    std::unique_ptr<LogHandler> next_;
};

class FatalError : public LogHandler {
public:
    using LogHandler::LogHandler;
private:
    bool HandlePush(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::FatalError) {
            return false;
        }
        throw std::runtime_error("Fatal Error!");
    }
};

class Error : public LogHandler {
public:
    using LogHandler::LogHandler;
private:
    bool HandlePush(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Error) {
            return false;
        }
        std::ofstream out("error.txt");
        out << "Error" << "\n";
        return true;
    }
};

class Warning : public LogHandler {
public:
    using LogHandler::LogHandler;
private:
    bool HandlePush(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Warning) {
            return false;
        }
        std::cout << "Warning!" << "\n";
        return true;
    }
};

class Unknown : public LogHandler {
public:
    using LogHandler::LogHandler;
private:
    bool HandlePush(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Unknown) {
            return false;
        }
        throw std::runtime_error("Unknown!");
        return true;
    }
};

class FatalErrorMessage : public LogMessage {
    Type type() const override { return Type::FatalError; }
    const std::string& message() const override {
        return "Fatal Error";
    }
};

class ErrorMessage : public LogMessage {
    Type type() const override { return Type::Error; }
    const std::string& message() const override {
        return "Error";
    }
};

class WarningMessage : public LogMessage {
    Type type() const override { return Type::Warning; }
    const std::string& message() const override {
        return "Warning";
    }
};

class UnknownMessage : public LogMessage {
    Type type() const override { return Type::Unknown; }
    const std::string& message() const override {
        return nullptr;
    }
};

int main()
{
    auto unknown_handler = std::make_unique<Unknown>(nullptr);
    auto warning_handler = std::make_unique<Warning>(std::move(unknown_handler));
    auto error_handler = std::make_unique<Error>(std::move(warning_handler));
    auto fatal_error_handler = std::make_unique<FatalError>(std::move(error_handler));
    try {
        fatal_error_handler->receive(WarningMessage());
        fatal_error_handler->receive(ErrorMessage());
        fatal_error_handler->receive(FatalErrorMessage());
        fatal_error_handler->receive(UnknownMessage());
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << "\n";
    }

}
