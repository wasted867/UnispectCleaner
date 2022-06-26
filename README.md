# UnispectCleaner
Takes your unispect dump and converts it to a nice json for easy updating also allows you to generate a struct for selected offsets 

## Building
Build As x64 Debug-Release

## Issues
For games that have obfuscated names this may not work as intened because the names could be diffrent every update

## Example Usage:
```
    PrintStruct(json_data, "GameWorld", "[Class] EFT.GameWorld", 
    {
        /*
            First = Real Offset Name
            Second = Final Name For Offset In Struct If Empty It Will Use Real Offset Name
        */
        {"int32_0x0C", "LineCast"},
        {"gClass0B71_0x18", "ExitController"},
        {"LootList", ""},
        {"RegisteredPlayers", ""}
    });
```

## Example Output:
```
struct GameWorld
{
    uint64_t LineCast = 0x0C;
    uint64_t ExitController = 0x18;
    uint64_t LootList = 0x68;
    uint64_t RegisteredPlayers = 0x88;
}GameWorld;
```
