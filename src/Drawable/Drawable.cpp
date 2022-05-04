#include "Drawable.h"

namespace rt
{
    Drawable::Drawable(const std::shared_ptr<Material>& material):
        m_Material(material)
    {
    }
}
