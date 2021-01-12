#include "mapgenerator.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

MapGenerator::MapGenerator()
{
    file_name = "static_map.txt";
}

void MapGenerator::GenerateMap(TURN_MATRIX const & matrix, uint8_t const vertex_count)
{
    SerializeTurnMatrix(matrix, vertex_count);
    CreateCsvExport();
}

void MapGenerator::SerializeTurnMatrix(const TURN_MATRIX& matrix, const uint8_t vertex_count)
{
    for (int i = 0; i < vertex_count; i++)
    {
        csv_string.push_back("{");
        for (int j = 0; j < vertex_count; j++)
        {
            SerializeMoveInfo(matrix[i][j]);

            if (j < vertex_count - 1)
            {
                csv_string.push_back(", ");
            }

        }

        if (i < vertex_count - 1)
        {
            csv_string.push_back("},\n");
        }
        else
        {
            csv_string.push_back("}\n");
        }
    }
}

void MapGenerator::SerializeMoveInfo(const MOVE_INFO move)
{
    std::string str = "";

    // Template: {10U, edFrontMid, adBackward}

    std::stringstream stream;
    stream << std::setw(5) << std::setfill(' ') << move.weight;
    std::string weigth_string = stream.str();

    str += "{" + weigth_string + "U, ";

    switch (move.turning)
    {
        case edFrontLeft:    str += " edFrontLeft";      break;
        case edFrontMid:     str += "  edFrontMid";      break;
        case edFrontRight:   str += "edFrontRight";      break;
        case edRearLeft:     str += "  edRearLeft";      break;
        case edRearMid:      str += "   edRearMid";      break;
        case edRearRight:    str += " edRearRight";      break;
        default:
            break;
    }
    str += ", ";

    if (move.direction == adBackward){  str += "adBackward";    }
    else{                               str += " adForward";    }
    str += "}";

    csv_string.push_back(str);
}

void MapGenerator::CreateCsvExport()
{
    std::ofstream csv_file;

    csv_file.open(file_name);

    csv_file << csvHeader;
    for (std::size_t i = 0; i < csv_string.size(); i++)
    {
        csv_file << csv_string[i];
    }

    csv_file.close();
}
