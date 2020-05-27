#include "Physics.h"

Physics::Physics()
{
	// Create the dynamics world 
	world = new rp3d::CollisionWorld();


	// Add the time difference in the accumulator 
	/* NÄMNS I 9.3 AV DOKUMENTATIONEN
	// Compute the time interpolation factor
decimal factor = accumulator / timeStep;

// Compute the interpolated transform of the rigid body
rp3d::Transform interpolatedTransform = Transform::interpolateTransforms(prevTransform, currTransform, factor);
	*/

	// Create the box shape 
	//rp3d::BoxShape* box = CreateBoxShape();

	//// Mass of the collision shape (in kilograms) 
	//rp3d::decimal mass = rp3d::decimal(4.0);
	//rp3d::RigidBody* rb = CreateRigidBody(0.5f, 0.5f, 2.0f, 2.0f, 2.0f);
	//// Add the collision shape to the rigid body
	//rp3d::ProxyShape* proxyShape;
	//proxyShape = rb->addCollisionShape(box, rb->getTransform(), mass);

	// While there is enough accumulated time to take 
	// one or several physics steps 
}

Physics::~Physics()
{
}


//rp3d::RigidBody* Physics::CreateRigidBody(float linearDamping, float angularDamping, float x, float y, float z)
//{
//    // Initial position and orientation of the rigid body 
//    rp3d::Vector3 initPosition(x, y, z);
//    rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
//    rp3d::Transform transform(initPosition, initOrientation);
//
//    // Create a rigid body in the world 
//    rp3d::RigidBody* body;
//  //  body = world->createRigidBody(transform);
//
//    // Change the type of the body to Dynamic
//    body->setType(rp3d::BodyType::DYNAMIC);
//
//    body->setLinearDamping(linearDamping);
//    body->setAngularDamping(angularDamping);
//
//    return body;
//}


rp3d::ConvexMeshShape* Physics::ConvertMeshToConvexShape(Mesh* mesh)
{
	//const int size = 10;
	//// Array with the vertices coordinates of the convex mesh 
	//float vertices[24];
	//vertices[0] = -size; vertices[1] = -size; vertices[2] = size;
	//vertices[3] = size; vertices[4] = -size; vertices[5] = size;
	//vertices[6] = size; vertices[7] = -size; vertices[8] = -size;
	//vertices[9] = -size; vertices[10] = -size; vertices[11] = -size;
	//vertices[12] = -size; vertices[13] = size; vertices[14] = size;
	//vertices[15] = size; vertices[16] = size; vertices[17] = size;
	//vertices[18] = size; vertices[19] = size; vertices[20] = -size;
	//vertices[21] = -size; vertices[22] = size; vertices[23] = -size;

	//// Array with the vertices indices for each face of the mesh 
	//int indices[24];
	//indices[0] = 0; indices[1] = 3; indices[2] = 2; indices[3] = 1;
	//indices[4] = 4; indices[5] = 5; indices[6] = 6; indices[7] = 7;
	//indices[8] = 0; indices[9] = 1; indices[10] = 5; indices[11] = 4;
	//indices[12] = 1; indices[13] = 2; indices[14] = 6; indices[15] = 5;
	//indices[16] = 2; indices[17] = 3; indices[18] = 7; indices[19] = 6;
	//indices[20] = 0; indices[21] = 4; indices[22] = 7; indices[23] = 3;

	//// Description of the six faces of the convex mesh 
	//rp3d::PolygonVertexArray::PolygonFace* polygonFaces = new rp3d::PolygonVertexArray::PolygonFace[6];
	//rp3d::PolygonVertexArray::PolygonFace* face = polygonFaces;
	//for (int f = 0; f < 6; f++) {

	//    // First vertex of the face in the indices array 
	//    face->indexBase = f * 4;

	//    // Number of vertices in the face 
	//    face->nbVertices = 4;

	//    face++;
	//}

	//// Create the polygon vertex array 
	//rp3d::PolygonVertexArray* polygonVertexArray = new rp3d::PolygonVertexArray(
	//    8, vertices, 3 * sizeof(float),
	//    indices, sizeof(int), 6, polygonFaces,
	//    rp3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
	//    rp3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE
	//);

	//// Create the polyhedron mesh 
	//rp3d::PolyhedronMesh* polyhedronMesh = new rp3d::PolyhedronMesh(polygonVertexArray);

	//// Create the convex mesh collision shape 
	//rp3d::ConvexMeshShape* convexMeshShape = new rp3d::ConvexMeshShape(polyhedronMesh);
	//return convexMeshShape;

	const int vertexCount = mesh->vertexes.size();
	const int indexCount = mesh->indices.size();
	const int faceCount = indexCount / 3;

	float* vertices = new float[vertexCount * 3];
	int* indices = new int[indexCount];
	int index = 0;

	for (auto i = mesh->vertexes.begin(); i < mesh->vertexes.end(); i++)
	{
		DirectX::XMFLOAT3 v = (*i).position;
		vertices[index + 0] = v.x;
		vertices[index + 1] = v.y;
		vertices[index + 2] = v.z;
		index += 3;
	}

	index = 0;
	for (auto i = mesh->indices.begin(); i < mesh->indices.end(); i++)
	{
		int id = static_cast<int>(*i);
		indices[index] = id;
		index++;
	}

	rp3d::PolygonVertexArray::PolygonFace* polygonFaces = new rp3d::PolygonVertexArray::PolygonFace[faceCount];
	rp3d::PolygonVertexArray::PolygonFace* face = polygonFaces;

	for (size_t i = 0; i < faceCount; i++)
	{
		face->indexBase = i * 3;
		face->nbVertices = 3;
		face++;
	}

	rp3d::PolygonVertexArray* vertexArray = new rp3d::PolygonVertexArray(
		vertexCount,      // nr of vertices
		&vertices,                   // vertices start ? 
		sizeof(float) * 3,          // vertices stride? 
		&indices,                    // indexes start?       
		sizeof(int),                // index stride
		faceCount,                      // nr o faces
		polygonFaces,                       // polygon faces
		rp3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
		rp3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE
	);

	Logger::Write(std::to_string(sizeof(*vertexArray)));
	Logger::Write(std::to_string(sizeof(vertexArray)));
	Logger::Write(std::to_string(sizeof(rp3d::PolygonVertexArray)));

	rp3d::PolyhedronMesh* polyhedron = new rp3d::PolyhedronMesh(vertexArray);
	rp3d::ConvexMeshShape* convexMesh = new rp3d::ConvexMeshShape(polyhedron);
	return convexMesh;
}

rp3d::ProxyShape* Physics::AddCollision(rp3d::ConvexMeshShape* shape, rp3d::RigidBody* body, float massValue)
{
	// Add the collision shape to the rigid body
	rp3d::decimal mass = rp3d::decimal(massValue);
	rp3d::ProxyShape* proxyShape;
	proxyShape = body->addCollisionShape(shape, body->getTransform(), mass);
	return proxyShape;
}

rp3d::Transform Physics::ConvertTransform(const Transform& transform)
{
	DirectX::XMFLOAT3 pos, rot;
	DirectX::XMStoreFloat3(&pos, transform.GetPosition());
	DirectX::XMStoreFloat3(&rot, transform.GetRotation());

	rp3d::Vector3 position(pos.x, pos.y, pos.z);
	rp3d::Quaternion rotation = rp3d::Quaternion::fromEulerAngles(rot.x, rot.y, rot.z);
	return rp3d::Transform(position, rotation);
}

rp3d::Vector3 Physics::Convert(DirectX::XMVECTOR v3)
{
	DirectX::XMFLOAT3 v;
	DirectX::XMStoreFloat3(&v, v3);
	return rp3d::Vector3(v.x, v.y, v.z);
}

rp3d::CollisionBody* Physics::CreateCollisionBodyBox(Object* object)
{
	rp3d::Transform rpTransform = ConvertTransform(object->GetTransform());
	rp3d::CollisionBody* body = world->createCollisionBody(rpTransform);// rp3d::Transform::identity());

	const rp3d::Vector3 extends = Convert(DirectX::XMVectorScale(object->GetLocalBounds().GetSize(), 0.5f));
	rp3d::BoxShape* box = new rp3d::BoxShape(extends);

	bodies.push_back(body);
	body->addCollisionShape(box, rp3d::Transform::identity());
	return body;
}

rp3d::CollisionBody* Physics::CreateCollisionBodyConvex(Object* object)
{
	rp3d::Transform rpTransform = ConvertTransform(object->GetTransform());
	rp3d::CollisionBody* body = world->createCollisionBody(rp3d::Transform::identity());
	rp3d::ConvexMeshShape* meshShape;

	auto found = meshShapeCache.find(object->GetMesh());
	if (found == meshShapeCache.end())
	{
		meshShape = ConvertMeshToConvexShape(object->GetMesh());
		meshShapeCache.insert({ object->GetMesh(), meshShape });
	}
	else
	{
		meshShape = (*found).second;
	}

	bodies.push_back(body);
	body->addCollisionShape(meshShape, rpTransform);
	return body;
}

rp3d::CollisionBody* Physics::CreateCollisionBodyCapsule(Object* object, float radius, float xOffset, float zOffset, float height)
{
	rp3d::Transform rpTransform = ConvertTransform(object->GetTransform());
	rp3d::Vector3 pos = rpTransform.getPosition();
	pos.x += xOffset;
	pos.z += zOffset;
	rpTransform.setPosition(pos);

	rp3d::CollisionBody* body = world->createCollisionBody(rpTransform);
	rp3d::CapsuleShape* capsule = new rp3d::CapsuleShape(radius, height);

	bodies.push_back(body);
	body->addCollisionShape(capsule, rp3d::Transform::identity());
	return body;
}

void Physics::UpdateCollisionBody(rp3d::CollisionBody* body, const Transform& transform)
{
	body->setTransform(ConvertTransform(transform));
}

RaycastHit Physics::Raycast(DirectX::XMVECTOR start, DirectX::XMVECTOR end)
{
	rp3d::Vector3 s = Convert(start);
	rp3d::Vector3 e = Convert(end);

	rp3d::Ray ray(s, e);

	RaycastCallback callback;
	callback.hitInfo.hit = false;

	world->raycast(ray, &callback);

	if (callback.hitInfo.hit)
		Logger::Write("PPPRRIIIIIIIINNNNNTT");

	return callback.hitInfo;
}
