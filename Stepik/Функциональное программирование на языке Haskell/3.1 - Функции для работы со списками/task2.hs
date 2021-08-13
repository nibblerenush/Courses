nTimes:: a -> Int -> [a]


nTimes symbol count =
    let
        helper symbol 0 result = result
        helper symbol count result = helper symbol (count - 1) (symbol : result)
    in
        helper symbol count []
