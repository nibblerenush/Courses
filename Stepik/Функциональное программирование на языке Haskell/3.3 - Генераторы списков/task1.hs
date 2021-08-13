fibStream :: [Integer]

fibStream =
    let
        helper prev1 prev2 = (prev1 + prev2) : helper (prev1 + prev2) prev1
    in
        0 : helper 0 1
