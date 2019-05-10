#ifndef __BULK_INCLUDED
#define __BULK_INCLUDED

#include <chrono>
#include <iterator>

#if __has_include(<filesystem>)

#include <filesystem>

#elif __has_include(<experimental/filesystem>)

#include <experimental/filesystem>

namespace std {

namespace filesystem = experimental::filesystem;

}

#else

#error "Cannot find <filesystem> header for inclusion"

#endif

#include <fstream>
#include <iostream>

#include "iface.hpp"

namespace usr {

using Cmd = std::string;
using Bulk = std::vector<Cmd>;

namespace internal {

class Writer: public utl::Subscriber<Bulk> {
	std::ostream &o;

	void update(Bulk const &v) override final {
		auto const &cend{std::cend(v)};
		auto cit{std::cbegin(v)};

		if (cit != cend) {
			o << "bulk: ";

			for (auto const &clast{std::prev(cend)}; cit != clast; ++cit) {
				o << *cit << ", ";
			}

			o << *cit << std::endl;
		}
	}

protected:
	Writer(std::ostream &o = std::cout): o{o} {
		o.exceptions(std::ostream::badbit | std::ostream::failbit | std::ostream::eofbit);
	}
};

class PreFiler {
	std::filesystem::path const name;
	std::ofstream file;

protected:
	PreFiler(): name{std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) + ".log"}, file{name} {
	}

	void finalize() {
		file.flush();
	}

	std::filesystem::path const &path() const noexcept {
		return name;
	}

	std::ostream &ostream() noexcept {
		return file;
	}
};

} // namespace internal

struct Logger final: internal::Writer {
	Logger(std::ostream &o = std::cout): Writer{o} {
	}
};

class Filer final: internal::PreFiler, public internal::Writer {
	void finalize() override {
		PreFiler::finalize();
	}

public:
	Filer(): Writer{ostream()} {
	}

	using PreFiler::path;
};


class Interpreter final: public utl::Publisher<Bulk>, public utl::Subscriber<Cmd> {
	unsigned nesting;

	class BulkWrapper {
		unsigned const bulksz;
		Bulk data;

	public:
		BulkWrapper(unsigned const bulksize): bulksz{bulksize} {
			data.reserve(bulksz);
		}

		BulkWrapper(BulkWrapper &&wrapper) noexcept: bulksz{std::move(wrapper.bulksz)}, data{std::move(wrapper.data)} {
			data.reserve(bulksz);
		}

		Bulk *operator ->() noexcept {
			return &data;
		}

		operator Bulk const &() const noexcept {
			return data;
		}

		unsigned bulksize() const noexcept {
			return bulksz;
		}
	} data;

	void update(Cmd const &s) override final {
		if (s == "{") {
			if (nesting++ || !data->size()) {
				return;
			}
		} else {
			if (s == "}") {
				if (!nesting || --nesting) {
					return;
				}
			} else {
				data->push_back(s);

				if (nesting || data->size() != data.bulksize()) {
					return;
				}
			}
		}

		bulkupdate(BulkWrapper{std::move(data)});
	}

	void finalize() noexcept override final {
		if (!nesting && !data->empty()) {
			bulkupdate(BulkWrapper{std::move(data)});
		}
	}

public:
	Interpreter(unsigned const bulksize): nesting{}, data{bulksize} {
	}

	~Interpreter() override final {
		finalize();
	}
};

class Reader final: public utl::Publisher<std::string> {
	std::istream &istream;

public:
	Reader(std::istream &istream = std::cin): istream{istream} {
	}

	void run() {
		std::string line;

		while (std::getline(istream, line)) {
			bulkupdate(line);
		}

		bulkfinalize();
	}
};

} // namespace usr

#endif
