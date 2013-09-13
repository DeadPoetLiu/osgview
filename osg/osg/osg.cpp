#include <osg/Geometry>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osg/Point>
#include <osg/PolygonOffset>
#include <osgUtil/SmoothingVisitor>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>










#include<osgViewer/Viewer>
#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>

#include <osgGA/TerrainManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/io_utils>
#include <osg/ShapeDrawable>

#include <osgText/Text>

#include <sstream>
#include <cmath>

#include<osgDB/ReadFile>
#include "CommonFunctions.h"
#include "PointIntersector.h"
int count=0;
osg::Vec3d ab[2];

double distance(osg::Vec3d & a, osg::Vec3d & b){
	double x=a[0]-b[0];
	double y=a[1]-b[1];
	double z=a[2]-b[2];

	return sqrt(x*x+y*y+z*z);
}

const osg::Vec4 normalColor(1.0f, 1.0f, 1.0f, 1.0f);
const osg::Vec4 selectedColor(1.0f, 0.0f, 0.0f, 1.0f);

class SelectModelHandler : public osgGA::GUIEventHandler
{
public:
    SelectModelHandler( osg::Camera* camera )
    : _selector(0), _camera(camera) {}
    
    osg::Geode* createPointSelector()
    {
        osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(1);
        (*colors)[0] = selectedColor;
        
        _selector = new osg::Geometry;
        _selector->setDataVariance( osg::Object::DYNAMIC );
        _selector->setUseDisplayList( false );
        _selector->setUseVertexBufferObjects( true );
        _selector->setVertexArray( new osg::Vec3Array(1) );
        _selector->setColorArray( colors.get() );
        _selector->setColorBinding( osg::Geometry::BIND_OVERALL );
        _selector->addPrimitiveSet( new osg::DrawArrays(GL_POINTS, 0, 1) );
        
        osg::ref_ptr<osg::Geode> geode = new osg::Geode;
        geode->addDrawable( _selector.get() );
        geode->getOrCreateStateSet()->setAttributeAndModes( new osg::Point(10.0f) );
        geode->getOrCreateStateSet()->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
        return geode.release();
    }
    
    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
    {
        if ( ea.getEventType()!=osgGA::GUIEventAdapter::RELEASE ||
             ea.getButton()!=osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ||
             !(ea.getModKeyMask()&osgGA::GUIEventAdapter::MODKEY_CTRL) )
            return false;
        
        osgViewer::View* viewer = dynamic_cast<osgViewer::View*>(&aa);
        if ( viewer )
        {
            osg::ref_ptr<PointIntersector> intersector =
                new PointIntersector(osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
            osgUtil::IntersectionVisitor iv( intersector.get() );
            viewer->getCamera()->accept( iv );
            
            if ( intersector->containsIntersections() )
            {
                osg::Vec3Array* selVertices = dynamic_cast<osg::Vec3Array*>( _selector->getVertexArray() );
                if ( !selVertices ) return false;
                
                PointIntersector::Intersection result = *(intersector->getIntersections().begin());
                selVertices->front() = result.getWorldIntersectPoint();
				ab[count]=result.getWorldIntersectPoint();
				count++;
				if(count==2){
					std::cout<<ab[0]<<std::endl<<ab[1]<<std::endl<<distance(ab[0],ab[1])<<std::endl<<std::endl;
					count=0;
				}
                selVertices->dirty();
                _selector->dirtyBound();
            }
        }
        return false;
    }
    
protected:
    osg::ref_ptr<osg::Geometry> _selector;
    osg::observer_ptr<osg::Camera> _camera;
};













osg::Camera* createHUDCamera( double left, double right, double 
bottom, double top )
{   int screenWidth=500;
    int screenHeight=500;
	float width  = screenWidth * 0.35f;
float height = screenHeight * 0.35f;
osg::ref_ptr<osg::Camera> camera = new osg::Camera;
camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
camera->setClearMask( GL_DEPTH_BUFFER_BIT );
camera->setRenderOrder( osg::Camera::POST_RENDER );
camera->setAllowEventFocus( false );
camera->setProjectionMatrix(
osg::Matrix::ortho2D(left, right, bottom, top) );
camera->getOrCreateStateSet()->setMode(
GL_LIGHTING, osg::StateAttribute::OFF );


osg::Texture2D* renderTexture = new osg::Texture2D;
renderTexture->setTextureSize(screenWidth, screenHeight);
renderTexture->setInternalFormat(GL_RGBA);
renderTexture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
renderTexture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);


osg::Image* hudImage;
       hudImage = osgDB::readImageFile("Images/osg256.png");
osg::Texture2D* HUDTexture = new osg::Texture2D;
 renderTexture->setImage(hudImage);
 osg::ref_ptr<osg::Geometry> screenQuad;
screenQuad = osg::createTexturedQuadGeometry(osg::Vec3(),
                                             osg::Vec3(width, 0.0, 0.0),
                                             osg::Vec3(0.0, height, 0.0));

 osg::ref_ptr<osg::Geode> quadGeode = new osg::Geode;
quadGeode->addDrawable(screenQuad.get());

osg::StateSet *quadState = quadGeode->getOrCreateStateSet();
quadState->setTextureAttributeAndModes(0, renderTexture, osg::StateAttribute::ON);

camera->addChild(quadGeode.get());

return camera.release();
}
int main( int argc, char **argv )

{
	 osg::ArgumentParser arguments(&argc,argv);
	osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);
	osg::ref_ptr<osg::Group> group= new osg::Group;
	group->addChild(scene.get());
	group->addChild(createHUDCamera(0,1000,1000,0));
osgViewer::Viewer viewer;
 osg::ref_ptr<SelectModelHandler> selector = new SelectModelHandler( viewer.getCamera() );

 group->addChild( selector->createPointSelector() );
 viewer.addEventHandler( selector.get() );

 osg::CullSettings::CullingMode mode = viewer.getCamera()->getCullingMode();
    viewer.getCamera()->setCullingMode( mode & (~osg::CullSettings::SMALL_FEATURE_CULLING) );


viewer.setSceneData(group.get() );

return viewer.run();

}