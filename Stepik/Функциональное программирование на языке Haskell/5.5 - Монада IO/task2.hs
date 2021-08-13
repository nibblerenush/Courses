import Data.List
import System.Directory

main' :: IO ()
main' = do
    putStr "Substring: "
    subStr <- getLine
    filePaths <- getDirectoryContents "."

    if (subStr == "")
        then
            putStrLn "Canceled"
        else
            mapM_ (\x -> (putStrLn $ "Removing file: " ++ x) >> removeFile x) $ filter (\x -> isInfixOf subStr x) filePaths
