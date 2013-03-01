
#include <theia/graphics/material.h>

void theia::Material::Apply(const MaterialState& material)
{
  if (material.emissiveParam) { material.shader->SetParameter(material.emissiveParam, material.Ke); }
  if (material.ambientParam)  { material.shader->SetParameter(material.ambientParam,  material.Ka); }
  if (material.diffuseParam)  { material.shader->SetParameter(material.diffuseParam,  material.Kd); }
  if (material.specularParam) { material.shader->SetParameter(material.specularParam, material.Ks); }
}
