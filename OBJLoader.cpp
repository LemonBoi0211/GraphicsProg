#include "OBJLoader.h"

vector<Vertex> OBJLoader::LoadOBJ(const string& FolderLoc, const string& Filename, string& AmbiantLoc, string& Diffloc, string& specLoc, string& NormalLoc, vector<uint> &indices)
{
	string line; //next line of text from the file
	string MatlibName; //name of the mtl lib
	string MaterialOfMesh; //name of the ntl to use
	string MeshName; //name of the mesh / object

	vector<vec3> VertPositions; //temp storage for vertex position
	vector<vec3> VertNormals; //temp storage for vertex normals
	vector<vec3> VertTextureCoords; //temp storage for vertex TexCoords
	vector<Vertex> FinalVerts; //list of verts, normal and tex coords

	ifstream file;
	string FileLoc = FolderLoc + "/" + Filename;
	const char* fileNameChar = FileLoc.c_str( );
	file.open(fileNameChar, ifstream::in);

	if ( file.is_open( ) )
	{
		while ( file.good( ) )
		{
			getline(file, line);
			if ( line[ 0 ] != '#' )
			{
				string FirstWord = line.substr(0, line.find(' '));
				if ( FirstWord == "mtllib" )
				{
					MatlibName = line.substr(line.find(' ') + 1, line.find('\n'));
				}
				else if ( FirstWord == "o" )
				{
					MeshName = line.substr(line.find(' '), line.find('\n'));
				}
				else if ( FirstWord == "v" )
				{
					string VertValues = line.substr(line.find(' '), line.find('\n'));
					float x, y, z;
					sscanf_s(VertValues.c_str( ), "%f %f %f", &x, &y, &z);
					VertPositions.push_back(vec3(x, y, z));
				}
				else if ( FirstWord == "vn" )
				{
					string VertNormValues = line.substr(line.find(' '), line.find('\n'));
					float x, y, z;
					sscanf_s(VertNormValues.c_str( ), "%f %f %f", &x, &y, &z);
					VertNormals.push_back(vec3(x, y, z));
				}
				else if ( FirstWord == "vt" )
				{
					string VertTexValues = line.substr(line.find(' '), line.find('\n'));
					float x, y, z;
					sscanf_s(VertTexValues.c_str( ), "%f %f %f", &x, &y, &z);
					VertTextureCoords.push_back(vec3(x, y, z));
				}
				else if ( FirstWord == "usemtl1" )
				{
					MaterialOfMesh = line.substr(line.find(' ') + 1, line.find('\n'));
					LoadMaterial(FolderLoc + "/" + MatlibName, AmbiantLoc, Diffloc, specLoc, NormalLoc);
				}
				else if ( FirstWord == "f" )
				{
					string FaceValues = line.substr(line.find(' '), line.find('\n'));

					Vertex vertsInFace[ 3 ];

					unsigned int TmpPosition[ 3 ]{}, TmpTexCoords[ 3 ]{}, TmpNormals[ 3 ]{};

					sscanf_s(FaceValues.c_str( ), "%d/%d/%d %d/%d/%d %d/%d/%d",
						&TmpPosition[ 0 ], &TmpTexCoords[ 0 ], &TmpNormals[ 0 ],
						&TmpPosition[ 1 ], &TmpTexCoords[ 1 ], &TmpNormals[ 1 ],
						&TmpPosition[ 2 ], &TmpTexCoords[ 2 ], &TmpNormals[ 2 ]);

					vertsInFace[ 0 ].Position = VertPositions[ TmpPosition[ 0 ] - 1 ];
					vertsInFace[ 0 ].TextureCoord = VertTextureCoords[ TmpTexCoords[ 0 ] - 1 ];
					vertsInFace[ 0 ].Normal = VertNormals[ TmpNormals[ 0 ] - 1 ];

					vertsInFace[ 1 ].Position = VertPositions[ TmpPosition[ 1 ] - 1 ];
					vertsInFace[ 1 ].TextureCoord = VertTextureCoords[ TmpTexCoords[ 1 ] - 1 ];
					vertsInFace[ 1 ].Normal = VertNormals[ TmpNormals[ 1 ] - 1 ];

					vertsInFace[ 2 ].Position = VertPositions[ TmpPosition[ 2 ] - 1 ];
					vertsInFace[ 2 ].TextureCoord = VertTextureCoords[ TmpTexCoords[ 2 ] - 1 ];
					vertsInFace[ 2 ].Normal = VertNormals[ TmpNormals[ 2 ] - 1 ];

					FinalVerts.push_back(vertsInFace[ 0 ]);
					FinalVerts.push_back(vertsInFace[ 1 ]);
					FinalVerts.push_back(vertsInFace[ 2 ]);
				}

				
				
			}
			
		}
		for ( int i = 0; i < FinalVerts.size( ); i++ )
		{
			indices.push_back(i);
		}

		return FinalVerts;
	}
	else
	{
		cerr << "Unable to Load Text File: " << FolderLoc + "/" + Filename << endl;
	}

}

void OBJLoader::LoadMaterial(const string& MatLibLoc, string& AmbiantLoc, string& DiffLoc, string& specLoc, string& NormalLoc)
{
	

	ifstream file;
	const char* fileNameChar = MatLibLoc.c_str( );
	file.open(fileNameChar, ifstream::in);
	string line;
	string MatName;

	if ( file.is_open())
	{
		while ( file.good())
		{
			getline(file, line);
			if ( line[ 0 ] != '#' )
			{
				string FirstWord = line.substr(0, line.find(' '));
				if ( strstr(FirstWord.c_str( ), "newmtl") )
				{
					MatName = line.substr(line.find(' ') + 1, line.find('\n'));
				}
				else if ( strstr(FirstWord.c_str( ), "map_Ka") )
				{
					AmbiantLoc = line.substr(line.find(' ') + 1, line.find('\n'));
				}
				else if ( strstr(FirstWord.c_str( ), "map_Kd") )
				{
					DiffLoc = line.substr(line.find(' ') + 1, line.find('\n'));
				}
				else if ( strstr(FirstWord.c_str( ), "map_Ks") )
				{
					specLoc = line.substr(line.find(' ') + 1, line.find('\n'));
				}
				else if ( strstr(FirstWord.c_str( ), "map_bump") )
				{
					NormalLoc = line.substr(line.find(' ') + 1, line.find('\n'));
				}
			}
		}
	}
	else
	{
		cerr << "Unable to Load Text File: " << MatLibLoc << endl;
	}
	file.close( );
}
