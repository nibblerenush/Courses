perms :: [a] -> [[a]]

perms [] = [[]]
perms x =
    let
        helper [] _ = []
        helper [x] [] = [[x]]
        helper (x:xs) prev = ( map (\item -> x:item) (helper (xs ++ prev) []) ) ++ ( helper xs (x:prev) )
    in
        helper x []
