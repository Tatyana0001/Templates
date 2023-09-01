#include <iostream>
#include <fstream>
#include <vector>

class Observer {
public:
    virtual void onWarning(const std::string& message) const {}
    virtual void onError(const std::string& message) const {}
    virtual void onFatalError(const std::string& message) const {}
};

class WarningObserver : public Observer {
public:
        void onWarning(const std::string& message) const override {
        std::cout << "Warning: " << message << "\n";
    }
};

class ErrorObserver : public Observer {
public:
    ErrorObserver() = delete;
    explicit ErrorObserver(const std::string& filename) : filename_(filename) {}
    void onError(const std::string& message) const override {
        std::ofstream fout;
        fout.open(filename_, std::ios_base::app);
        if(fout.is_open()) {
            fout << "Error: " << message << "\n";
            fout.close();
        }
        else { std::cerr << "File not found " << "\n"; }
    }
private:
    std::string filename_;
};

class FatalErrorObserver : public Observer {
public:
    FatalErrorObserver(const std::string& filename) : filename_(filename) {}
    void onFatalError(const std::string& message) const override {
        std::cout << "Fatal Error: " << message << "\n";
        std::ofstream fout(filename_);
        if (fout.is_open()) {
            fout << "Fatal Error: " << message << "\n";
            fout.close();
        }
        else { std::cerr << "File not found " << "\n"; }
    }
private:
    std::string filename_;
};

class Observed{
public:
    void warning(const std::string& message) const {
        for (auto observer : WarningObservers) {
            observer->onWarning(message);
        }
    }
    void error(const std::string& message) const {
        for (auto observer : ErrorObservers) {
            observer->onError(message);
        }
    }
    void fatalError(const std::string& message) const {
        for (auto observer : FatalErrorObservers) {
            observer->onFatalError(message);
        }
    }
    void AddWarningObservers(const Observer& observer) {
        WarningObservers.push_back(&observer);
    }
    void AddErrorObservers(const Observer& observer) {
        ErrorObservers.push_back(&observer);
    }
    void AddFatalErrorObservers(const Observer& observer) {
        FatalErrorObservers.push_back(&observer);
    }
private:
    std::vector <const Observer*> WarningObservers;
    std::vector <const Observer*> ErrorObservers;
    std::vector <const Observer*> FatalErrorObservers;
};

int main()
{
    Observed obs;
    obs.AddWarningObservers(WarningObserver());
    obs.AddErrorObservers(ErrorObserver("error.txt"));
    obs.AddFatalErrorObservers(FatalErrorObserver("fatalerror.txt"));
    obs.warning("Warning message");
    obs.error("Error message");
    obs.fatalError("Fatal Error message");
    return 0;
}
