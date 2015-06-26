/* 
 * File:   ColladaModel.hpp
 * Author: tku
 *
 * Created on 18 grudzień 2013, 19:47
 */

#ifndef COLLADAMODEL_H
#define	COLLADAMODEL_H

#include <string>
#include <map>
#include <vector>

#include "NGE/Parsers/pugixml.hpp"
#include "NGE/Tools/Logger.hpp"

namespace NGE {
	namespace Geometry {
		namespace Models {

			class ColladaModel {
			  public:
				bool Initialize(const std::string& filename);
				bool LoadAsset();
				bool LoadGeometries();

				// TODO: Remove that after testing
				void PrintAsset();

			  protected:

				/**
				 * Enumeration type of possible up axis.
				 */
				enum UP_AXIS {
					X_UP, Y_UP, Z_UP
				};

				/**
				 * Struct that describes the contributor.
				 */
				struct collada_contributor {
					std::string author;
					std::string authoring_tool;
					std::string comments;
					std::string copyright;
					std::string source_data;

					void Print() {
						std::cout << "<contributor>\n" <<
								"\tauthor: " << author << "\n" <<
								"\tauthoring_tool: " << authoring_tool << "\n" <<
								"\tcomments: " << comments << "\n" <<
								"\tcopyright: " << copyright << "\n" <<
								"\tsource_data: " << source_data << "\n" <<
								"</contributor>" << std::endl;


					}
				};

				/**
				 * Struct that describes the asset.
				 */
				struct collada_asset {
					std::vector<collada_contributor> contributor;
					std::string created;
					std::string keywords;
					std::string modified;
					std::string revision;
					std::string subject;
					std::string title;
					// name:std::string / meter:float
					std::pair<std::string, float> unit;
					UP_AXIS upAxis;

					void Print() {
						std::cout << "<asset>\n";

						for (unsigned int i = 0; i < contributor.size(); ++i)
							contributor[i].Print();

						std::cout <<
								"\tcreated: " << created << "\n" <<
								"\tmodified: " << modified << "\n" <<
								"\tkeywords: " << keywords << "\n" <<
								"\trevision: " << revision << "\n" <<
								"\tsubject: " << subject << "\n" <<
								"\ttitle: " << title << "\n" <<
								"\tunit name: " << unit.first << "\n" <<
								"\tunit meter: " << unit.second << "\n" <<
								"\tup_axis: " << upAxis << "\n" <<
								"</asset>" << std::endl;

					}
				};

				/**
				 * Name and path of the *.dae file.
				 */
				std::string fileName;

				/**
				 * XML document that contains COLLADA file.
				 */
				pugi::xml_document colladaFile;

				/**
				 * Result of initial XML parsing.
				 */
				pugi::xml_parse_result fileParseResult;

				/**
				 * Asset node in colada file. It should be only one in the file.
				 * It contains basic information about the file.
				 */
				collada_asset asset;
			};
		}
	}
}

#endif	/* COLLADAMODEL_HPP */

