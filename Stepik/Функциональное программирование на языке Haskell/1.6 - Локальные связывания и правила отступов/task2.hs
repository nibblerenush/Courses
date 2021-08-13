sum'n'count :: Integer -> (Integer, Integer)

sum'n'count 0 = (0, 1)
sum'n'count x =
    let
        abs_x = abs (x)
        helper 0 sum count = (sum, count)
        helper num sum count = 
            let
                num_div = div (num) (10)
                num_mod = mod (num) (10)
            in
                helper (num_div) (sum + num_mod) (count + 1)
        
    in
        helper (abs_x) (0) (0)
