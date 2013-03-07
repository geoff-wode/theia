
#include <theia/graphics/material.h>

theia::MaterialState::MaterialState(ShaderPtr shader)
  : Ke(0), Ka(0.1f), Kd(0.5f), Ks(0.6f, 0.6f, 0.6f, 32),
    shader(shader),
    emissiveParam(shader->GetParameter("Material.Ke")),
    ambientParam(shader->GetParameter("Material.Ka")),
    diffuseParam(shader->GetParameter("Material.Kd")),
    specularParam(shader->GetParameter("Material.Ks"))
{
}

void theia::Material::Apply(const MaterialState& material)
{
  if (material.emissiveParam) { material.shader->SetParameter(material.emissiveParam, material.Ke); }
  if (material.ambientParam)  { material.shader->SetParameter(material.ambientParam,  material.Ka); }
  if (material.diffuseParam)  { material.shader->SetParameter(material.diffuseParam,  material.Kd); }
  if (material.specularParam) { material.shader->SetParameter(material.specularParam, material.Ks); }
}
