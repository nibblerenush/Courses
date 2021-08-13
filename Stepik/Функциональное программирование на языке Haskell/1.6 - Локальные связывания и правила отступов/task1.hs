seqA :: Integer -> Integer

seqA 0 = 1
seqA 1 = 2
seqA 2 = 3
seqA n =
    let
        helper prev1 prev2 prev3 2 = prev1
        helper prev1 prev2 prev3 n = helper (prev1 + prev2 - 2 * prev3) (prev1) (prev2) (n - 1)
    in helper 3 2 1 n
