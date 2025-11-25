#pragma once 

//own
#include "basicTypes.h"
#include "typeTraits.h"
#include "limits.h"

//@export
namespace natl {
	constexpr inline Size defaultRandomSeed = 0xBAADDEFEC8EDC0DE;
	template<typename RandomGenerator>
	concept IsRandomGenerator = requires(RandomGenerator rg) {
		{ RandomGenerator() };
		{ RandomGenerator(declval<Size>()) };
		{ rg.seed(declval<Size>()) };
		{ rg.seed() };
		{ rg.random() } -> natl::SameAs<Size>;
	};

	template<typename RandomGenerator>
		requires(IsRandomGenerator<RandomGenerator>)
	struct UtilRandomGenerator {
		[[no_unique_address]] RandomGenerator rg;
	public:
		constexpr UtilRandomGenerator(const RandomGenerator& rgIn) noexcept : rg(rgIn) {}
		constexpr UtilRandomGenerator(RandomGenerator&& rgIn) noexcept : rg(forward<RandomGenerator>(rgIn)) {}
		constexpr UtilRandomGenerator(Size seed = defaultRandomSeed) noexcept : rg(seed) {}
		constexpr ~UtilRandomGenerator() noexcept = default;
		constexpr void seed(Size seed = defaultRandomSeed) noexcept {
			rg.seed(seed);
		}
		constexpr Size random() noexcept {
			return rg.random();
		}
		constexpr Size randomUI64() noexcept {
			return rg.random();
		};
		constexpr Size randomUI64(Size min, Size max) noexcept {
			f64 unit = static_cast<f64>(rg.random()) / static_cast<f64>(Limits<ui64>::max());
			return min + static_cast<Size>((max - min + 1) * unit);
		};
		constexpr i64 randomI64() noexcept {
			return static_cast<i64>(randomUI64());
		}
		constexpr i64 randomI64(i64 min, i64 max) noexcept {
			f64 unit = static_cast<f64>(rg.random()) / static_cast<f64>(Limits<ui64>::max());
			return min + static_cast<i64>((max - min + 1) * unit);
		}
		constexpr f64 randomF64() noexcept {
			constexpr f64 inv = 1.0 / static_cast<f64>(Limits<ui64>::max());
			return static_cast<f64>(rg.random()) * inv;
		}
		constexpr f64 randomF64(f64 min, f64 max) noexcept {
			return min + randomF64() * (max - min);
		}
		constexpr Bool randomBool() noexcept {
			return (randomUI64() & 1) != 0;
		}
	};

	struct Xorshift64 {
	private:
		ui64 s;

	public:
		constexpr Xorshift64(Size seed = defaultRandomSeed) noexcept : s(seed) {}
		constexpr ~Xorshift64() noexcept = default;
		constexpr void seed(Size seed = defaultRandomSeed) noexcept {
			s = seed;
		}
		constexpr Size random() noexcept {
			s ^= s << 13;
			s ^= s >> 7;
			s ^= s << 17;
			return s;
		}
	};
	static_assert(IsRandomGenerator<Xorshift64>);

	struct SplitMix64 {
	private:
		ui64 s;

	public:
		constexpr SplitMix64(Size seed = defaultRandomSeed) noexcept : s(seed) {}
		constexpr ~SplitMix64() noexcept = default;
		constexpr void seed(Size seed = defaultRandomSeed) noexcept {
			s = seed;
		}
		constexpr Size random() noexcept {
			s += 0x9E3779B97F4A7C15;
			ui64 sp = s;
			sp = (sp ^ (sp >> 30)) * 0xBF58476D1CE4E5B9;
			sp = (sp ^ (sp >> 27)) * 0x94D049BB133111EB;
			return sp ^ (sp >> 31);
		}
	};
	static_assert(IsRandomGenerator<SplitMix64>);


	template<typename SeedRandomGenerator = SplitMix64>
		requires(IsRandomGenerator<SeedRandomGenerator>)
	struct Xoshiro256ss {
	private:
		ui64 s[4];

	public:
		constexpr Xoshiro256ss(Size seed = defaultRandomSeed) noexcept {
			this->seed(seed);
		}
		constexpr ~Xoshiro256ss() noexcept = default;

	private:
		constexpr ui64 rol(ui64 x, ui64 k) noexcept {
			return (x << k) | (x >> (64 - k));
		}

	public:
		constexpr void seed(Size seed = defaultRandomSeed) noexcept {
			SeedRandomGenerator srg(seed);
			s[0] = srg.random();
			s[1] = srg.random();
			s[2] = srg.random();
			s[3] = srg.random();
		}
		constexpr Size random() noexcept {
			const ui64 result = rol(s[1] * 5, 7) * 9;
			const ui64 t = s[1] << 17;
			s[2] ^= s[0];
			s[3] ^= s[1];
			s[1] ^= s[2];
			s[0] ^= s[3];
			s[2] ^= t;
			s[3] = rol(s[3], 45);
			return result;
		}
	};
	static_assert(IsRandomGenerator<Xoshiro256ss<>>);

	using StandardRNG = UtilRandomGenerator<Xoshiro256ss<SplitMix64>>;

	// defined in timer.cpp
	Size rand() noexcept;

	using RandomFunctionType = Size(&)(void);
	struct RandomGeneratorFunction {
	private:
		RandomFunctionType randomFunc;

	public:
		constexpr RandomGeneratorFunction(RandomFunctionType randomFuncIn) noexcept : randomFunc(randomFuncIn) {}
		constexpr RandomGeneratorFunction(Size) noexcept : randomFunc(rand) {}
		constexpr ~RandomGeneratorFunction() noexcept = default;
		constexpr void seed(Size) noexcept {}
		constexpr Size random() noexcept { return randomFunc(); }
	};

	constexpr RandomGeneratorFunction randomGenerator(RandomFunctionType randomFuncIn = rand) noexcept {
		return RandomGeneratorFunction(randomFuncIn);
	};

	template<typename RandomGenerator>
		requires(IsRandomGenerator<RandomGenerator>)
	struct RandomGeneratorRef {
	private:
		RandomGenerator& rg;

	public:
		constexpr RandomGeneratorRef(RandomGenerator& rgIn) noexcept : rg(rgIn) {}
		constexpr RandomGeneratorRef(Size seed = defaultRandomSeed) noexcept : rg(getStaticRg()) {
			rg.seed(seed);
		}
		constexpr ~RandomGeneratorRef() noexcept = default;
		constexpr void seed(Size seed = defaultRandomSeed) noexcept {
			rg.seed(seed);
		}
		constexpr Size random() noexcept {
			return rg.random();
		}

	private:
		constexpr RandomGenerator& getStaticRg() noexcept {
			static RandomGenerator rg{};
			return rg;
		}
	};

	template<typename RandomGenerator>
		requires(IsRandomGenerator<RandomGenerator>)
	constexpr RandomGeneratorRef<RandomGenerator> randomGeneratorRef(RandomGenerator& rg) noexcept {
		return RandomGeneratorRef<RandomGenerator>(rg);
	};
}