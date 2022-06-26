#include <iostream>
#include <Windows.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <Commdlg.h>

#include "../HeaderFiles/json.hpp"

std::string GetFileName()
{
    OPENFILENAME ofn;

    char szFile[100];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0json\0*.json\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    GetOpenFileName(&ofn);
    return (std::string)ofn.lpstrFile;
}

std::string GetOffset(std::string line)
{
    std::string removed_path = line.substr(line.find_first_of("[\\") + 1);
    removed_path = removed_path.substr(0, removed_path.find_first_of("]\\"));

    if (removed_path == "Interface" || removed_path == "Struct" || removed_path == "Enum")
    {
        return "0x0";
    }

    return std::string("0x" + removed_path);
}

std::string GetName(std::string line)
{
    std::string removed_path = line.substr(line.find_first_of("]\\") + 1);
    removed_path = removed_path.substr(0, removed_path.find_first_of(":\\"));

    return removed_path;
}

std::string GetType(std::string line)
{
    std::string removed_path = line.substr(line.find_last_of(":\\") + 1);

    return removed_path;
}

std::string CleanLine(std::string line)
{
    size_t s = line.find("[S]");
    if (s != std::string::npos)
    {
        line.erase(s, 3);
    }

    size_t c = line.find("[C]");
    if (c != std::string::npos)
    {
        line.erase(c, 3);
    }

    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    return line;
}

bool DumpToJson()
{
    system("cls");
    printf("Select Unispect Dump\n");
    std::string file_name = GetFileName();

    nlohmann::ordered_json json;

    int current_line = 0;
    std::string class_name;
    std::ifstream game_dump;

    game_dump.open(file_name.c_str());

    if (game_dump.is_open())
    {
        std::string line;
        while (!game_dump.eof())
        {
            current_line++;

            getline(game_dump, line);
            if ((line.find("[Class]", 0)) != std::string::npos)
            {

                class_name = line.substr(0, line.find_first_of(":\\") - 1);
                continue;
            }
            else if (class_name == "")
            {
                continue;
            }

            std::string cleaned_line = CleanLine(line);
            json[class_name][GetName(cleaned_line)] = { GetOffset(cleaned_line), GetType(cleaned_line) };
        }
        game_dump.close();

        std::ofstream out("CleanedDump.json");
        out << std::setw(4) << json << std::endl;
        out.close();
    }
    return true;
}

enum JsonData
{
    Offset = 0,
    Type = 1
};

void PrintStruct(nlohmann::json json_data, std::string structure_name, std::string class_name, std::vector<std::pair<std::string, std::string>> data)
{
    std::vector<std::string> stored_data;

    printf("struct %s \n{\n", structure_name.c_str());

    for (int i = 0; i < data.size(); i++)
    {
        json_data[class_name][data[i].first].get_to(stored_data);

        if (data[i].second != "")
        {
            printf("    uint64_t %s = %s;\n", data[i].second.c_str(), stored_data[JsonData::Offset].c_str());
        }
        else
        {
            printf("    uint64_t %s = %s;\n", data[i].first.c_str(), stored_data[JsonData::Offset].c_str());
        }
    }

    printf("}%s;\n", structure_name.c_str());
}

void GenerateStruct()
{
    system("cls");
    printf("Select Cleaned Game Dump\n");
    std::string file_name = GetFileName();
    system("cls");

    std::ifstream ifstreammm(file_name.c_str());
    nlohmann::json json_data = nlohmann::json::parse(ifstreammm);

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

    return;
}

int main()
{
    system("cls");

    int option;
    printf("Eft Offset Updater\n\n[1] Clean Dump\n[2] Generate Struct\n[3] Both\n\n");
    std::cin >> option;
    system("cls");

    switch (option)
    {
    case 1:
        DumpToJson();
        break;
    case 2:
        GenerateStruct();
        break;
    case 3:
        DumpToJson();
        GenerateStruct();
        break;
    default:
        printf("Enter Correct Option\n");
        Sleep(1000);
        system("cls");
        main();
        break;
    }

    printf("\n\n\nPress Insert To Continue\n");

    while (true)
    {
        if (GetAsyncKeyState(VK_INSERT) & 1) break;
        Sleep(100);
    }
    main();

	return 1;
}