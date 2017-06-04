#include "skinned_mesh.hpp"

#include <iostream>

#include "texture.hpp"

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

void SkinnedMesh::setBoneTransformations(GLuint shaderProgram, GLfloat currentTime)
{
    std::vector<glm::mat4> Transforms;
        boneTransform((float)currentTime, Transforms);
        for (unsigned int i = 0; i < Transforms.size(); ++i)
		{
			const std::string name = "gBones[" + std::to_string(i) + "]";
			//std::cout << glm::to_string(Transforms[i]) << std::endl;
			GLuint boneTransform = glGetUniformLocation(shaderProgram, name.c_str());
			//glUniformMatrix4fv(boneTransform, 1, GL_TRUE, (const GLfloat*)(Transforms[i]));
            Transforms[i] = glm::transpose(Transforms[i]);
            glUniformMatrix4fv(boneTransform, 1, GL_TRUE, glm::value_ptr(Transforms[i]));
		}
}

glm::mat3 OLDaiMatrix3x3ToGlm(const aiMatrix3x3& from)
{
	/*glm::mat4 to;
	to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1;
	to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2;
	to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3;
	return glm::transpose(to);*/
	glm::mat3x3 out;
	//assert(sizeof(out) == sizeof(v));
	memcpy(&out, &from, sizeof(from));
	return out;
	//return glm::transpose(out);
}

glm::mat4 OLDaiMatrix4x4ToGlm(const aiMatrix4x4& from)
{
	/*glm::mat4 to;
	to[0][0] = (GLfloat)from.a1; to[0][1] = (GLfloat)from.b1;  to[0][2] = (GLfloat)from.c1; to[0][3] = (GLfloat)from.d1;
	to[1][0] = (GLfloat)from.a2; to[1][1] = (GLfloat)from.b2;  to[1][2] = (GLfloat)from.c2; to[1][3] = (GLfloat)from.d2;
	to[2][0] = (GLfloat)from.a3; to[2][1] = (GLfloat)from.b3;  to[2][2] = (GLfloat)from.c3; to[2][3] = (GLfloat)from.d3;
	to[3][0] = (GLfloat)from.a4; to[3][1] = (GLfloat)from.b4;  to[3][2] = (GLfloat)from.c4; to[3][3] = (GLfloat)from.d4;
	return glm::transpose(to);*/
	glm::mat4x4 out;
	//assert(sizeof(out) == sizeof(v));
	memcpy(&out, &from, sizeof(from));
	return out;
	//return glm::transpose(out);
}


glm::mat3 aiMatrix3x3ToGlm(const aiMatrix3x3 &from)
{
	glm::mat3 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2;	to[2][0] = from.a3; 
	to[0][1] = from.b1; to[1][1] = from.b2;	to[2][1] = from.b3; 
	to[0][2] = from.c1; to[1][2] = from.c2;	to[2][2] = from.c3; 
	
	return to;
}

glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4 &from)
{
	glm::mat4 to;
	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = from.a1; to[1][0] = from.a2;	to[2][0] = from.a3; to[3][0] = from.a4;
	to[0][1] = from.b1; to[1][1] = from.b2;	to[2][1] = from.b3; to[3][1] = from.b4;
	to[0][2] = from.c1; to[1][2] = from.c2;	to[2][2] = from.c3; to[3][2] = from.c4;
	to[0][3] = from.d1; to[1][3] = from.d2;	to[2][3] = from.d3; to[3][3] = from.d4;
	return to;
}

/* Adds a new Bone */
void SkinnedMesh::VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
	for (unsigned int i = 0; i < NUM_BONES_PER_VERTEX; ++i)
		if (Weights[i] == 0.0)
		{
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
}

/* Init */
SkinnedMesh::SkinnedMesh() : 
    currentAnimation(0)
{
	m_VAO = 0;
	for (unsigned int i = 0; i < NUM_VBs; ++i)
	{
		m_Buffers[i] = 0;
	}
	m_NumBones = 0;
	m_pScene = NULL;
}

SkinnedMesh::~SkinnedMesh()
{
	Clear();
}

void SkinnedMesh::Clear()
{
	/* Textures must not be deleted because they are GLuint */

	/* Deletes VBOs */
	if (m_Buffers[0] != 0)
		glDeleteBuffers(NUM_VBs, m_Buffers);
	/* Deletes VAO */
	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

/* loads the model */
bool SkinnedMesh::loadMesh(const std::string& fileName)
{
	/* Deletes the previous loaded mesh(if it exists) */
	Clear();

	/* Create the VAO */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	/* Create VBOs for vertices attributes */
	glGenBuffers(NUM_VBs, m_Buffers);

	/* Return value */
	bool ret = false;

	m_pScene = m_Importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    
    
	if (m_pScene)
	{
		/* Get transformation matrix for nodes(vertices relative to boes) */
		aiMatrix4x4 tp1 = m_pScene->mRootNode->mTransformation;
		m_GlobalInverseTransform = aiMatrix4x4ToGlm(tp1);
		m_GlobalInverseTransform =  glm::inverse(m_GlobalInverseTransform);
		

		
		ret = InitFromScene(m_pScene, fileName);
	}
	else
	{
		std::cout << "Error parsing : " << fileName << " : " << m_Importer.GetErrorString() << std::endl;
        return false;
	}

	/* Make sure the VAO is not changed from the outside */
	glBindVertexArray(0);

	std::cout << "NumIndices : " << m_Entries[0].NumIndices << std::endl;
	std::cout << "BaseVertex : " << m_Entries[0].BaseVertex << std::endl;
	std::cout << "BaseIndex : " << m_Entries[0].BaseIndex << std::endl;
	std::cout << "MaterialIndex : " << m_Entries[0].MaterialIndex << std::endl << std::endl << std::endl;

	std::cout << "NumIndices : " << m_Entries[1].NumIndices << std::endl;
	std::cout << "BaseVertex : " << m_Entries[1].BaseVertex << std::endl;
	std::cout << "BaseIndex : " << m_Entries[1].BaseIndex << std::endl;
	std::cout << "MaterialIndex : " << m_Entries[1].MaterialIndex << std::endl;

	return ret;
}

unsigned int SkinnedMesh::getNumAnimations()
{
    return m_pScene->mNumAnimations;
}

void SkinnedMesh::setAnimation(unsigned int a)
{
    if(a >= 0 && a < getNumAnimations())
        currentAnimation = a;
}

bool SkinnedMesh::InitFromScene(const aiScene* pScene, const std::string& fileName)
{
	/* Resize the mesh & texture vectors */
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);
	
	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<VertexBoneData> Bones;
	std::vector<unsigned int> Indices;

	unsigned int NumVertices = 0;
	unsigned int NumIndices = 0;

	/* Count the number of vertices and indices */
	for (unsigned int i = 0; i < m_Entries.size(); ++i)
	{
		m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_Entries[i].BaseVertex = NumVertices;
		m_Entries[i].BaseIndex = NumIndices;

		NumVertices += pScene->mMeshes[i]->mNumVertices;
		NumIndices += m_Entries[i].NumIndices;
	}

	// Reserve space in the vectors for the vertex attributes and indices
	Positions.reserve(NumVertices);
	Normals.reserve(NumVertices);
	TexCoords.reserve(NumVertices);
	Bones.resize(NumVertices);
	Indices.reserve(NumIndices);

	

	/* Initialize the meshes in the scene one by one */
	for (unsigned int i = 0; i < m_Entries.size(); ++i)
	{
		/* get mesh */
		const aiMesh* paiMesh = pScene->mMeshes[i];
		/* init the mesh */
		InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
	}
	
	/* init the material */
	if (!InitMaterials(pScene, fileName))
		return false;
	
	/* Generate and populate the buffers with vertex attributes and indices */
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/*
	for (unsigned int i = 0; i < Colors.size(); ++i)
	{
		std::cout << Colors[i].x << " - " << Colors[i].y << " - " << Colors[i].z << std::endl;
	}*/
	/*
	m_ColorBuffer = 0;
	glGenBuffers(1, &m_ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors[0]) * Colors.size(), &Colors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, 0);
	*/
	/*
	for (unsigned int i = 0; i < Bones.size(); ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
			std::cout << Bones[i].Weights[j] << ",  ";
		std::cout << std::endl;
	}*/

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	

	if (glGetError() == GL_NO_ERROR)
		return true;
	else
		return false;
}

void SkinnedMesh::InitMesh(unsigned int MeshIndex,
	const aiMesh* paiMesh,
	std::vector<glm::vec3>& Positions,
	std::vector<glm::vec3>& Normals,
	std::vector<glm::vec2>& TexCoords,
	std::vector<VertexBoneData>& Bones,
	std::vector<unsigned int>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	
	std::cout << "Num vertices = " << paiMesh->mNumVertices << std::endl;
	/* Populize the vertex attribute vectors */
	for (unsigned int i = 0; i < paiMesh->mNumVertices; ++i)
	{
		/* Get pos normal texCoord */
		
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		/* store pos normal texCoord */
		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
	
		if (paiMesh->HasNormals())
		{
			const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
			Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		}
		/*
		const aiVector3D* pColor = paiMesh->HasVertexColors(0) ? &(aiVector3D(paiMesh->mColors[0][i], paiMesh->mColors[0][i].g, paiMesh->mColors[0][i].b)) : &Zero3D;
		Colors.push_back(glm::vec3(pColor->x, pColor->y, pColor->z));
		*/
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	/* Load bones */
	LoadBones(MeshIndex, paiMesh, Bones);

	/* Populate the index buffer */
	for (unsigned int i = 0; i < paiMesh->mNumFaces; ++i)
	{
		const aiFace& Face = paiMesh->mFaces[i];
		/* if mNumIndices != 3 then worning */
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}

void SkinnedMesh::LoadBones(unsigned int MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
	/* Load bones one by one */
	
	for (unsigned int i = 0; i < pMesh->mNumBones; ++i)
	{
		unsigned int BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
		{
			/* allocate an index for the new bone */
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);

			aiMatrix4x4 tp1 = pMesh->mBones[i]->mOffsetMatrix;
			m_BoneInfo[BoneIndex].BoneOffset = aiMatrix4x4ToGlm(tp1);
			//m_BoneInfo[BoneIndex].BoneOffset = glm::mat4(tp1.a1, tp1.a2, tp1.a3, tp1.a4, tp1.b1, tp1.b2, tp1.b3, tp1.b4, tp1.c1, tp1.c2, tp1.c3, tp1.c4, tp1.d1, tp1.d2, tp1.d3, tp1.d4);

			//m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else
		{
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (unsigned int j = 0; j < pMesh->mBones[i]->mNumWeights; ++j)
		{
			//std::cout << pMesh->mBones[i]->mWeights. << std::endl;
			unsigned int VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}

bool SkinnedMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool ret = true;

	/* Initialize the materials */
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		/* Get the material */
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_Textures[i] = 0;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\")
				{
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				

				//m_Textures[i] = SOILTextureFromFile(FullPath);
				/*
				if (!m_Textures[i])
				{
					std::cout << "Error loading texture : " << FullPath << std::endl;
					ret = false;
				}
				else
				{
					std::cout << i << " - " << "loaded texture " << FullPath << std::endl;
				}*/
			}
		}
	}
	return ret;
}

void SkinnedMesh::render(const GLuint& shaderProgram)
{
	glBindVertexArray(m_VAO);

	for (unsigned int i = 0; i < m_Entries.size(); i++) {
		const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;
		
		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex]) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Textures[MaterialIndex]);


			/* the texture uniform */
			//GLint textureLocation = glGetUniformLocation(shaderProgram, "texture1");
			//glUniform1i(textureLocation, 0);
		}
		
		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_Entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);
	}

	glBindVertexArray(0);
}

unsigned int SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	
	
	
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) 
	{
		
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	
	assert(0);
	return 0;
}

unsigned int SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);
	return 0;
}

unsigned int SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	assert(0);
	return 0;
}

void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(AnimationTime, pNodeAnim);
		unsigned int NextPositionIndex = (PositionIndex + 1);
		assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
}

void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(AnimationTime, pNodeAnim);
		unsigned int NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
}

void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
		unsigned int NextScalingIndex = (ScalingIndex + 1);
		assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
}

void SkinnedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = m_pScene->mAnimations[currentAnimation];

	aiMatrix4x4 tp1 = pNode->mTransformation;
	//glm::mat4 NodeTransformation(tp1.a1, tp1.a2, tp1.a3, tp1.a4, tp1.b1, tp1.b2, tp1.b3, tp1.b4, tp1.c1, tp1.c2, tp1.c3, tp1.c4, tp1.d1, tp1.d2, tp1.d3, tp1.d4);
	glm::mat4 NodeTransformation = aiMatrix4x4ToGlm(tp1);
	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		glm::mat4 ScalingM;
		
		ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));
		//ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		aiMatrix3x3 tp = RotationQ.GetMatrix();

		//glm::mat4 RotationM(glm::mat3(tp.a1, tp.a2, tp.a3, tp.b1, tp.b2, tp.b3, tp.c1, tp.c2, tp.c3));
		//glm::mat3 RotationM3x3 = aiMatrix3x3ToGlm(tp);
		//glm::mat4 RotationM(RotationM3x3);
		glm::mat4 RotationM = aiMatrix3x3ToGlm(tp);

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM;
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));
		
		//TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM *ScalingM;
		
		
	}

	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		unsigned int BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void SkinnedMesh::boneTransform(float timeInSeconds, std::vector<glm::mat4>& Transforms)
{
	glm::mat4 Identity = glm::mat4();
	//Identity.InitIdentity();
	
	animDuration = (float)m_pScene->mAnimations[currentAnimation]->mDuration;

	/* Calc animation duration */
	unsigned int numPosKeys = m_pScene->mAnimations[currentAnimation]->mChannels[0]->mNumPositionKeys;
	animDuration = m_pScene->mAnimations[currentAnimation]->mChannels[0]->mPositionKeys[numPosKeys - 1].mTime;
	///std::cout << "Anim time actual : " << m_pScene->mAnimations[currentAnimation]->mChannels[0]->mPositionKeys[numPosKeys-1].mTime << std::endl;

	float TicksPerSecond = (float)(m_pScene->mAnimations[currentAnimation]->mTicksPerSecond != 0 ? m_pScene->mAnimations[currentAnimation]->mTicksPerSecond : 25.0f);
	//TicksPerSecond = 3;
	float TimeInTicks = timeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, animDuration);
	


	/*
	std::cout << "Ticks per second          = " << TicksPerSecond << std::endl;
	std::cout << "Time in seconds           = " << timeInSeconds << std::endl;
	std::cout << "Time in ticks             = " << TimeInTicks << std::endl;
	std::cout << "Animation time ticks      = " << AnimationTime << std::endl;
	std::cout << "Animation duration ticks  = " << m_pScene->mAnimations[0]->mDuration << "\n\n\n\n";
	*/
	ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

	Transforms.resize(m_NumBones);
	
	for (unsigned int  i = 0; i < m_NumBones; i++) {
		Transforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}

const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}
