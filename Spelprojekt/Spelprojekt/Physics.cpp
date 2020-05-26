#include "Physics.h"

Physics::Physics(float gX, float gY, float gZ, float currentTime, float lastFrame, float deltaTime)
{
    float accumulator = 0;

    // Gravity vector 
    gravity = rp3d::Vector3(gX, gY, gZ);

    // Create the dynamics world 
    world = new rp3d::DynamicsWorld(gravity);

    // Constant physics time step 
    const float timeStep = 1.0 / 60.0;

    // Get the current system time 
    long double currentFrameTime = currentTime;

    // Compute the time difference between the two frames 
    //Jag tycker att denna borde vara samma deltatime som application har, men den var definerad som "long double deltaTime" från början
    deltaTime = currentFrameTime - lastFrame;

    // Update the previous time 
    lastFrame = currentTime;

    // Add the time difference in the accumulator 
    accumulator += deltaTime;
    /* NÄMNS I 9.3 AV DOKUMENTATIONEN
    // Compute the time interpolation factor
decimal factor = accumulator / timeStep;

// Compute the interpolated transform of the rigid body
rp3d::Transform interpolatedTransform = Transform::interpolateTransforms(prevTransform, currTransform, factor);
    */

    // Create the box shape 
    rp3d::BoxShape* box = CreateBoxShape();

    // Mass of the collision shape (in kilograms) 
    rp3d::decimal mass = rp3d::decimal(4.0);
    rp3d::RigidBody* rb = CreateRigidBody(0.5f, 0.5f, 2.0f, 2.0f, 2.0f);
    // Add the collision shape to the rigid body
    rp3d::ProxyShape* proxyShape;
    proxyShape = rb->addCollisionShape(box, rb->getTransform(), mass);

    // While there is enough accumulated time to take 
    // one or several physics steps 
    while (accumulator >= timeStep) {

        // Update the Dynamics world with a constant time step 
        world->update(timeStep);

        // Decrease the accumulated time 
        accumulator -= timeStep;
    }
}

Physics::~Physics()
{
}

rp3d::RigidBody* Physics::CreateRigidBody(float linearDamping, float angularDamping, float x, float y, float z)
{
    // Initial position and orientation of the rigid body 
    rp3d::Vector3 initPosition(x, y, z);
    rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
    rp3d::Transform transform(initPosition, initOrientation);

    // Create a rigid body in the world 
    rp3d::RigidBody* body;
    body = world->createRigidBody(transform);

    // Change the type of the body to Dynamic
    body->setType(rp3d::BodyType::DYNAMIC);

    body->setLinearDamping(linearDamping);
    body->setAngularDamping(angularDamping);

    return body;
}

rp3d::BoxShape* Physics::CreateBoxShape()
{
    // Half extents of the box in the x, y and z directions 
    const rp3d::Vector3 halfExtents(2.0, 3.0, 5.0);

    // Create the box shape 
    rp3d::BoxShape* boxShape = new rp3d::BoxShape(halfExtents);

    return boxShape;
}

rp3d::ConvexMeshShape* Physics::ConvertMeshToConvexShape(Mesh* mesh)
{
    float* vertices = new float[mesh->vertexes.size() * 3];
    int* indices = new int[mesh->indices.size()];
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
        indices[index] = static_cast<int>(*i);
        index++;
    }
    
    int faces = mesh->indices.size() / 3;
    rp3d::PolygonVertexArray::PolygonFace* face = new rp3d::PolygonVertexArray::PolygonFace[faces];

    for (size_t i = 0; i < faces; i++)
    {
        face->indexBase = i * 3;
        face->nbVertices = 3;
        face++;
    }

    rp3d::PolygonVertexArray* vertexArray = new rp3d::PolygonVertexArray(
        mesh->vertexes.size(),      // nr of vertices
        vertices,                   // vertices start ? 
        3 * sizeof(float),          // vertices stride? 
        indices,                    // indexes start?       
        sizeof(int),                // index stride
        faces,                      // nr o faces
        face,                       // polygon faces
        rp3d::PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE, 
        rp3d::PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE
    );

    delete[] vertices;
    delete[] indices;

    rp3d::PolyhedronMesh* polyhedron = new rp3d::PolyhedronMesh(vertexArray);
    rp3d::ConvexMeshShape* convexMesh = new rp3d::ConvexMeshShape(polyhedron);
    return convexMesh;
}
