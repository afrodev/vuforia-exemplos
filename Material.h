//
//  Material.h
//  Created by DrNeurosurg on 18.01.13.
//
//

#ifndef _Material_h
#define _Material_h

#include "Vector.h"
enum MATERIALS
{
    EMERALD,
    JADE,
    OBSIDIAN,
    GOLD,
    TURQUOISE,
    PEARL,
    RUBY,
    BRASS,
    BRONZE,
    CHROME,
    COPPER,
    SILVER,
    BLACKPLASTIC,
    CYANPLASTIC,
    GREENPLASTIC,
    REDPLASTIC,
    WHITEPLASTIC,
    YELLOWPLASTIC,
    BLACKRUBBER,
    CYANRUBBER,
    GREENRUBBER,
    REDRUBBER,
    WHITERUBBER,
    YELLOWRUBBER
    
};

class Material
{

    
public:
    Material() {Default();return;}
    
    void Diffuse(vec3 diffuse)       {_diffuse = diffuse;}
    void Ambient(vec3 ambient)       {_ambient = ambient;}
    void Specular(vec3 specular)     {_specular = specular;}
    void Shininess(float shininess)  {_shininess = shininess;}
    void Emissive(vec3 emissive)      {_emissive = emissive;}
    
    const float* Diffuse()              { return _diffuse.Pointer();}
    const float* Ambient()              { return _ambient.Pointer();}
    const float* Specular()             { return _specular.Pointer();}
    const float Shininess()             { return _shininess;}
    const float* Emissive()             { return _emissive.Pointer();}
    
    Material(MATERIALS mat)
    {
        switch (mat) {
            case EMERALD:                Emerald();break;
            case    JADE:                Jade(); break;
            case OBSIDIAN:              Obsidian(); break;
            case GOLD:                  Gold(); break;
            case TURQUOISE:             Turquoise(); break;
            case PEARL:                 Pearl(); break;
            case RUBY:                  Ruby(); break;
            case BRASS:                 Brass();break;
            case BRONZE:                Bronze();break;
            case CHROME:                Chrome();break;
            case COPPER:                Copper(); break;
            case SILVER:                Silver();break;
            case BLACKPLASTIC:          BlackPlastic();break;
            case CYANPLASTIC:           CyanPlastic(); break;
            case GREENPLASTIC:          GreenPlastic(); break;
            case REDPLASTIC:            RedPlastic();break;
            case WHITEPLASTIC:          WhitePlastic(); break;
            case YELLOWPLASTIC:         YellowPlastic(); break;
            case BLACKRUBBER:           BlackRubber();break;
            case CYANRUBBER:            CyanRubber(); break;
            case GREENRUBBER:           GreenRubber();break;
            case REDRUBBER:             RedRubber(); break;
            case WHITERUBBER:           WhiteRubber(); break;
            case YELLOWRUBBER:          YellowRubber(); break;
                
            default:                    Default(); break;

        }
        
        return;
    }
    
private:
    vec3  _diffuse;
    vec3  _ambient;
    vec3  _specular;
    vec3  _emissive;
    float _shininess;
    float _alpha;
    
    void Emerald()
    {
        _ambient = vec3(0.0215, 0.1745, 0.0215);
        _diffuse = vec3(0.07568, 0.61424, 0.07568);
        _specular = vec3(0.633,	0.727811,	0.633);
        _shininess = 0.6 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Jade()
    {
        _ambient = vec3(0.135,	0.2225,	0.1575);
        _diffuse = vec3(0.54,	0.89,	0.63);
        _specular = vec3(0.316228,	0.316228	, 0.316228);
        _shininess = 12.8;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Obsidian()
    {
        _ambient = vec3(0.05375,	0.05	, 0.06625);
        _diffuse = vec3(0.18275,	0.17	, 0.22525);
        _specular = vec3(0.332741,	0.328634	, 0.346435);
        _shininess = 38.4;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Pearl()
    {
        _ambient = vec3(0.25,	0.20725,	0.2072);
        _diffuse = vec3(1.0,	0.829,	0.829);
        _specular = vec3(0.296648,	0.296648	, 0.296648);
        _shininess = 0.088 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Ruby()
    {
        _ambient = vec3(0.1745,	0.01175, 	0.01175);
        _diffuse = vec3(0.61424,	0.04136,	0.04136);
        _specular = vec3(0.727811,	0.626959, 	0.626959);
        _shininess = 0.6 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);

    }
    
    void Brass()
    {
        _ambient = vec3(0.329412,	0.223529, 	0.02745);
        _diffuse = vec3(0.780392,	0.568627	,  0.113725);
        _specular = vec3(0.992157,	0.941176	, 0.807843);
        _shininess = 0.21794872 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Bronze()
    {
        _ambient = vec3(0.2125,	0.1275,	0.054);
        _diffuse = vec3(0.714,	0.4284,	0.18144);
        _specular = vec3(0.393548,	0.271906	, 0.166721);
        _shininess = 0.2 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Chrome()
    {
        _ambient = vec3(0.25,	0.25, 	0.25);
        _diffuse = vec3(0.4,	0.4, 	0.4);
        _specular = vec3(0.774597,	0.774597,	0.774597);
        _shininess = 0.6 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Copper()
    {
        _ambient = vec3(0.19125,	0.0735,	0.0225);
        _diffuse = vec3(0.7038,	 0.27048, 	0.0828);
        _specular = vec3(0.256777,	0.137622, 	0.086014);
        _shininess = 0.1 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Silver()
    {
        _ambient = vec3(0.19225,	0.19225, 	0.19225);
        _diffuse = vec3(0.50754,	0.50754, 	0.50754);
        _specular = vec3(0.508273,	0.508273	, 0.508273);
        _shininess = 0.4 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void BlackPlastic()
    {
        _ambient = vec3(0,0,0);
        _diffuse = vec3(0.01, 0.01, 0.01);
        _specular = vec3(0.5, 0.5, 0.5);
        _shininess = 0.25 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void CyanPlastic()
    {
        _ambient = vec3(0.0, 0.1, 0.06);
        _diffuse = vec3(0.0,	0.50980392,	0.50980392);
        _specular = vec3(0.50196078, 	0.50196078,	0.50196078);
        _shininess = 0.25 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void GreenPlastic()
    {
        _ambient = vec3(0, 0, 0);
        _diffuse = vec3(0.1,	0.35	, 0.1);
        _specular = vec3(0.45,	0.55	,  0.45);
        _shininess = 0.25 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void RedPlastic()
    {
        _ambient = vec3(0, 0, 0);
        _diffuse = vec3(0.5, 0, 0);
        _specular = vec3(0.7, 0.6, 0.6);
        _shininess = 0.25 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void WhitePlastic()
    {
        _ambient = vec3(0,0, 0);
        _diffuse = vec3(0.55, 0.55, 0.55);
        _specular = vec3(0.7, 0.7, 0.7);
        _shininess = 0.25 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void YellowPlastic()
    {
        _ambient = vec3(0, 0, 0);
        _diffuse = vec3(0.5, 0.5, 0);
        _specular = vec3(0.6, 0.6, 0.5);
        _shininess = 0.25 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void BlackRubber()
    {
        _ambient = vec3(0.02, 0.02, 0.02);
        _diffuse = vec3(0.01, 0.01, 0.01);
        _specular = vec3(0.4, 0.4, 0.4);
        _shininess = 0.078125 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void CyanRubber()
    {
        _ambient = vec3(0, 0.05, 0.05);
        _diffuse = vec3(0.4, 0.4, 0.5);
        _specular = vec3(0.04, 0.7, 0.7);
        _shininess = 0.078125 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    
    void GreenRubber()
    {
        _ambient = vec3(0, 0.05, 0);
        _diffuse = vec3(0.4, 0.5, 0.4);
        _specular = vec3( 0.04, 0.7, 0.04);
        _shininess = 0.078125 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    
    void RedRubber()
    {
        _ambient = vec3(0.05, 0, 0);
        _diffuse = vec3(0.5, 0.4, 0.4);
        _specular = vec3(0.7, 0.04, 0.04);
        _shininess = 0.078125 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void WhiteRubber()
    {
        _ambient = vec3(0.05, 0.05, 0.05);
        _diffuse = vec3(0.5, 0.5, 0.5);
        _specular = vec3(0.7, 0.7, 0.7);
        _shininess = 0.078125 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void YellowRubber()
    {
        _ambient = vec3(0.05, 0.05, 0);
        _diffuse = vec3(0.5, 0.5, 0.4);
        _specular = vec3(0.7, 0.7, 0.04);
        _shininess = 0.25 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Default()
    {
        _ambient = vec3(0.2, 0.2, 0.2);
        _diffuse = vec3(0.8, 0.8, 0.8);
        _specular = vec3(0.7, 0.7, 0.7);
        _shininess = 0.078125 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Turquoise()
    {
        _ambient = vec3(0.1,	0.18725, 	0.1745);
        _diffuse = vec3(0.396,	0.74151, 	0.69102);
        _specular = vec3(0.297254,	0.30829, 	0.306678);
        _shininess = 0.1 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
    void Gold()
    {
        _diffuse = vec3(0.75, 0.606, 0.226);
        _ambient = vec3(0.24, 0.199, 0.075);
        _specular = vec3( 0.628, 0.555, 0.366);
        _shininess = 0.4 * 128;
        _emissive = vec3(0.0, 0.0, 0.0);
    }
    
};



#endif
