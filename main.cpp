#include <iostream>
#include <ctime>
#include <cstdint>
#include <set>
 
enum EEventPriority : uint8_t {
    MINOR = 0,
    MIDLE,
    MAJOR
};

struct EventKey {
    EEventPriority priority;
    std::time_t timestamp;

    bool operator<(const EventKey& other) const {
        if (priority == other.priority) {
            return timestamp < other.timestamp;
        }
        return priority < other.priority;
    }

    bool operator==(const EventKey& other) const {
        if (priority == other.priority) {
            return timestamp == other.timestamp;
        }
        return false;
    }
};

// TODO: implement
class EventStorage {
private:
    struct storage_t;
    std::set<storage_t> storage_;

    struct storage_t {
        EventKey key_;
        std::string message_;

        storage_t(const EventKey& key, const std::string& message)
            : key_(key)
            , message_(message)
        {}

        storage_t()
            : key_()
            , message_()
        {}

        bool operator==(const storage_t& other ) const {
            return key_ == other.key_;
        }

        std::string operator*() const {
            return message_;
        }

        bool operator<(const storage_t& other) const {
            return key_ < other.key_;
        }

        explicit operator bool() const {
            return !message_.empty();
        }
    };

public:
    EventStorage()
        : storage_()
    {}
    void AddEvent(const EventKey& key, const std::string& message) {
        storage_.emplace(key, message);
    }
    storage_t FindEvent(const EventKey& key) {
        auto result = storage_.find(storage_t{key,""});
        if (result != storage_.end()) {
            return *result;
        }
        return storage_t{};
    }

    std::string LowestEvent() const {
        if (storage_.empty()) {
            throw std::underflow_error("Storage is empty!");
        }
        return *(*(storage_.begin()));
    }

    std::string HighestEvent() const {
        if (storage_.empty()) {
            throw std::underflow_error("Storage is empty!");
        }
        return *(*(--storage_.end()));
    }

    void RemoveEvent(const EventKey& key) {
        storage_.erase(storage_t{key,""});
    }
};

int main(int, char**){
    EventStorage storage;

    // O(log(n))
    storage.AddEvent(
        {MAJOR, std::time(NULL) - 1}, "First Major Event"
    );

    storage.AddEvent(
        {MAJOR, std::time(NULL)}, "Second Major Event"
    );

    storage.AddEvent(
        {MINOR, std::time(NULL)}, "Minor Event");

    auto event = storage.FindEvent(EventKey{MAJOR, std::time(NULL) - 1});
    if(event)
        std::cout << *event << '\n'; //  "First Major Event"

    // O(1)
    // Prints "Minor Event"
    std::cout << storage.LowestEvent() << '\n';

    // O(1)
    // Prints "Second Major Event"
    std::cout << storage.HighestEvent() << '\n';

   // O(1)
    storage.RemoveEvent(EventKey{MAJOR, std::time(NULL)});

    // O(1)
    storage.RemoveEvent(EventKey{MINOR, std::time(NULL)});

    // // Both
    // // O(1)
    // Prints "First Major Event"
    std::cout << storage.LowestEvent() << '\n';
    std::cout << storage.HighestEvent() << '\n';

    return 0;
}
