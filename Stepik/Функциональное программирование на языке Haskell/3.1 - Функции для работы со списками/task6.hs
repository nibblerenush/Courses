groupElems :: Eq a => [a] -> [[a]]

groupElems [] = []
groupElems xs =
    let
        helper [] ys = ys : []
        helper (x : xs) [] = helper (xs) (x : [])
        helper (x : xs) ys = if x == last ys then helper (xs) (x : ys) else ys : helper xs (x : [])
    in
        helper xs []
