data LogLevel = Error | Warning | Info

cmp :: LogLevel -> LogLevel -> Ordering

cmp Error Error = EQ
cmp Error _ = GT
cmp _ Error = LT

cmp Info Info = EQ
cmp Info _ = LT
cmp _ Info = GT

cmp Warning Warning = EQ
