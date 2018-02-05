/*
Made by Lars Thomas Bremnes

/unlicense

See "Explanation.txt" for an explanation of this program, in text format.
*/


typedef unsigned __int64 u64;
typedef __int64 i64;
typedef unsigned int u32;

constexpr u64 Limit = (1ULL << 15); // This doesn't truly matter, but arrays need a size and the largest one needs to be at least 16090 large.

namespace OriginalU64 {
	static u64 FloorsAlreadyComputed[Limit + 1];

	u64 BestMaxThrows(u64);

	u64 MaxThrows(u64 FloorsLeft, u64 NextFloor) {
		if (FloorsLeft <= 2) {
			return FloorsLeft;
		} else {
			u64 Val = BestMaxThrows(FloorsLeft - NextFloor) + 1;
			if (Val < NextFloor) {
				Val = NextFloor;
			} return Val;
		}
	}

	u64 BestNextStep(u64 FloorsLeft) {
		if (FloorsLeft <= 2) {
			return 1;
		} else {
			u64 Min = (-1ULL);
			for (u64 i = 1; i < FloorsLeft; i++) {
				u64 Val = MaxThrows(FloorsLeft, i);
				if (Val < Min) {
					Min = Val;
				}
			} return Min;
		}
	}

	u64 BestMaxThrows(u64 FloorsLeft) {
		if (FloorsAlreadyComputed[FloorsLeft] == 0) {
			FloorsAlreadyComputed[FloorsLeft] = MaxThrows(FloorsLeft, BestNextStep(FloorsLeft));
		}
		return FloorsAlreadyComputed[FloorsLeft];
	}
}

namespace OriginalU32 {
	static u32 FloorsAlreadyComputed[Limit + 1];

	u32 BestMaxThrows(u32);

	u32 MaxThrows(u32 FloorsLeft, u32 NextFloor) {
		if (FloorsLeft <= 2) {
			return FloorsLeft;
		} else {
			u32 Val = BestMaxThrows(FloorsLeft - NextFloor) + 1;
			if (Val < NextFloor) {
				Val = NextFloor;
			} return Val;
		}
	}

	u32 BestNextStep(u32 FloorsLeft) {
		if (FloorsLeft <= 2) {
			return 1;
		} else {
			u32 Min = (-1UL);
			for (u32 i = 1; i < FloorsLeft; i++) {
				u32 Val = MaxThrows(FloorsLeft, i);
				if (Val < Min) {
					Min = Val;
				}
			} return Min;
		}
	}

	u32 BestMaxThrows(u32 FloorsLeft) {
		if (FloorsAlreadyComputed[FloorsLeft] == 0) {
			FloorsAlreadyComputed[FloorsLeft] = MaxThrows(FloorsLeft, BestNextStep(FloorsLeft));
		}
		return FloorsAlreadyComputed[FloorsLeft];
	}
}

namespace InlineBestNextStep {
	static u32 FloorsAlreadyComputed[Limit + 1];

	u32 BestMaxThrows(u32);

	u32 MaxThrows(u32 FloorsLeft, u32 NextFloor) {
		//if (FloorsLeft <= 2) { This is now redundant
			//return FloorsLeft;
		//} else {
			u32 Val = BestMaxThrows(FloorsLeft - NextFloor) + 1;
			if (Val < NextFloor) {
				Val = NextFloor;
			} return Val;
		//}
	}

	u32 BestMaxThrows(u32 FloorsLeft) {
		if (FloorsAlreadyComputed[FloorsLeft] == 0) {
			if (FloorsLeft <= 2) {
				FloorsAlreadyComputed[FloorsLeft] = FloorsLeft;
			} else {
				u32 Min = (-1UL);
				for (u32 i = 1; i < FloorsLeft; i++) {
					u32 Val = MaxThrows(FloorsLeft, i);
					if (Val < Min) {
						Min = Val;
					}
				}
				FloorsAlreadyComputed[FloorsLeft] = MaxThrows(FloorsLeft, Min);
			}
		}
		return FloorsAlreadyComputed[FloorsLeft];
	}
}

namespace InlineBestNextStepThenMaxThrowsOnce {
	static u32 FloorsAlreadyComputed[Limit + 1];

	u32 BestMaxThrows(u32);

	u32 MaxThrows(u32 FloorsLeft, u32 NextFloor) {
		//if (FloorsLeft <= 2) { This is now redundant
		//	return FloorsLeft;
		//} else {
			u32 Val = BestMaxThrows(FloorsLeft - NextFloor) + 1;
			if (Val < NextFloor) {
				Val = NextFloor;
			} return Val;
		//}
	}

	u32 BestMaxThrows(u32 FloorsLeft) {
		if (FloorsAlreadyComputed[FloorsLeft] == 0) {
			if (FloorsLeft <= 2) {
				FloorsAlreadyComputed[FloorsLeft] = FloorsLeft;
			} else {
				u32 Min = (-1UL);
				for (u32 i = 1; i < FloorsLeft; i++) {
					u32 Val = BestMaxThrows(FloorsLeft - i) + 1;
					if (Val < i) {
						Val = i;
					}
					if (Val < Min) {
						Min = Val;
					}
				}
				FloorsAlreadyComputed[FloorsLeft] = MaxThrows(FloorsLeft, Min);
			}
		} return FloorsAlreadyComputed[FloorsLeft];
	}
}

namespace InlineEverythingButBestMaxThrows {
	static u32 FloorsAlreadyComputed[Limit + 1];

	u32 BestMaxThrows(u32 FloorsLeft) {
		if (FloorsAlreadyComputed[FloorsLeft] == 0) {
			if (FloorsLeft <= 2) {
				FloorsAlreadyComputed[FloorsLeft] = FloorsLeft;
			} else {
				u32 Min = (-1UL);
				for (u32 i = 1; i < FloorsLeft; i++) {
					u32 Val = BestMaxThrows(FloorsLeft - i) + 1;
					if (Val < i) {
						Val = i;
					}
					if (Val < Min) {
						Min = Val;
					}
				}
				u32 Val = FloorsAlreadyComputed[FloorsLeft - Min] + 1;
				if (Val < Min) {
					Val = Min;
				} FloorsAlreadyComputed[FloorsLeft] = Val;
			}
		}
		return FloorsAlreadyComputed[FloorsLeft];
	}
}



int main(void) {
	u32 Val;
	Val = OriginalU64::BestMaxThrows(5365);	// 5365 seems completely safe, above and it stack overflows inconsistently
	Val = OriginalU32::BestMaxThrows(5850);	// 5850
	Val = InlineBestNextStep::BestMaxThrows(8045);	// 8045
	Val = InlineBestNextStepThenMaxThrowsOnce::BestMaxThrows(16090);	// 8045
	Val = InlineEverythingButBestMaxThrows::BestMaxThrows(16090);	// 8045


	#define DesiredFloor 16090ULL
	i64 Floor = DesiredFloor;
	u64 Throws = 0;
	while (Floor > 0) {
		Floor -= ++Throws;
	}

	Val = OriginalU32::BestMaxThrows(10);	// 5850
	Val = OriginalU32::BestMaxThrows(21);	// 5850
	return 0;
}