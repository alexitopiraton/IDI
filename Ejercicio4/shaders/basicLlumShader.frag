#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec4  vertexSCO;
in vec3  normalSCO;

uniform vec4 posFocusCamara;
uniform vec4 posFocusEscena1;
uniform vec4 posFocusEscena2;
uniform vec3 colorFocusCamara;
uniform vec3 colorFocusEscena;
uniform vec3 luzAmbiente;
uniform mat4 TG;
uniform mat4 View;
uniform float TGCarro;

out vec4 FragColor;



vec3 Ambient() {
  return luzAmbiente*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
  vec3 normSCO = normalize(normalSCO);

  // FOCO DE CAMARA
  vec3 L = posFocusCamara.xyz - vertexSCO.xyz;
  L = normalize(L);
  vec3 PhongCamara = Ambient() + Difus(normSCO, L, colorFocusCamara) + Especular(normSCO, L, vertexSCO.xyz, colorFocusCamara);

  // FOCO DE ESCENA1
  vec3 L2 = posFocusEscena1.xyz - vertexSCO.xyz;
  L2 = normalize(L2);
  vec3 LambertEscena1 = Difus(normSCO, L2, colorFocusEscena) + Especular(normSCO, L2, vertexSCO.xyz, colorFocusEscena);

  // FOCO DE ESCENA2
  vec3 L3 = posFocusEscena2.xyz - vertexSCO.xyz;
  L3 = normalize(L3);
  vec3 LambertEscena2 = Difus(normSCO, L3, colorFocusEscena) + Especular(normSCO, L3, vertexSCO.xyz, colorFocusEscena);


  vec3 fcolor = PhongCamara + LambertEscena1 + LambertEscena2;
    FragColor = vec4(fcolor, 1);
}
