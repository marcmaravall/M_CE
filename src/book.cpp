#include "book.h"

template<typename T>
T swapEndian(T value) {
    static_assert(sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8,
        "Only support for 2, 4 or 8 bits.");

    if constexpr (sizeof(T) == 2) {
        return ((value & 0xFF) << 8) | ((value >> 8) & 0xFF);
    }
    else if constexpr (sizeof(T) == 4) {
        return ((value & 0xFF) << 24) |
            (((value >> 8) & 0xFF) << 16) |
            (((value >> 16) & 0xFF) << 8) |
            ((value >> 24) & 0xFF);
    }
    else if constexpr (sizeof(T) == 8) {
        return ((value & 0xFFULL) << 56) |
            (((value >> 8) & 0xFFULL) << 48) |
            (((value >> 16) & 0xFFULL) << 40) |
            (((value >> 24) & 0xFFULL) << 32) |
            (((value >> 32) & 0xFFULL) << 24) |
            (((value >> 40) & 0xFFULL) << 16) |
            (((value >> 48) & 0xFFULL) << 8) |
            ((value >> 56) & 0xFFULL);
    }
}

template <typename T>
uint64_t extractBits(T value, int start, int count) {
    return (value >> start) & ((1ULL << count) - 1);
}

Move ConvertPolyglotToMove(const uint16_t& move) {
    Move current;
    current.from = 255;
    current.to = 255;
    current.promotion = 255;
    current.castling = false;
    current.mode = false;

    current.from = extractBits(move, 6, 6);
    current.to = extractBits(move, 0, 6);
    current.promotion = extractBits(move, 12, 3);

    return current;
}

Book::Book(const std::string& path) {
    if (path.empty()) {
        return;
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "ERROR: opening book cannot be read " << path << "\n";
        return;
    }

    PolyglotEntry entry;
    while (file.read(reinterpret_cast<char*>(&entry), sizeof(PolyglotEntry))) {
        entry.key = swapEndian(entry.key);
        entry.move = swapEndian(entry.move);
        entry.weight = swapEndian(entry.weight);
        entry.learn = swapEndian(entry.learn);

        entries.push_back(entry);
    }

    if (!std::is_sorted(entries.begin(), entries.end(),
        [](const PolyglotEntry& a, const PolyglotEntry& b) {
            return a.key < b.key;
        })) {
        std::cerr << "WARNING: Book entries are not sorted by key\n";
        std::sort(entries.begin(), entries.end(),
            [](const PolyglotEntry& a, const PolyglotEntry& b) {
                return a.key < b.key;
            });
    }

    std::cout << "Book loaded: " << entries.size() << " entries.\n";
}

std::pair<size_t, size_t> Book::FindRange(uint64_t key) const {
    auto lower = std::lower_bound(entries.begin(), entries.end(), key,
        [](const PolyglotEntry& entry, uint64_t k) {
            return entry.key < k;
        });

    auto upper = std::upper_bound(entries.begin(), entries.end(), key,
        [](uint64_t k, const PolyglotEntry& entry) {
            return k < entry.key;
        });

    return { static_cast<size_t>(lower - entries.begin()),
            static_cast<size_t>(upper - entries.begin()) };
}

std::vector<Move> Book::GetMoves(uint64_t key) const {
    auto [start, end] = FindRange(key);
    std::vector<Move> result;
    result.reserve(end - start);

    for (size_t i = start; i < end; ++i) {
        result.push_back(ConvertPolyglotToMove(entries[i].move));
    }

    return result;
}

Book::Book()
{

}
