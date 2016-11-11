#include "Helmet.h"

IMPLEMENT_SERIAL(Helmet, CObject, 1)

Helmet::Helmet(string name):Item(ItemTypes[HELMET], name)
{
}


Helmet::~Helmet()
{
}

Helmet::Helmet()
{
}

void Helmet::Serialize(CArchive & archive)
{
	// call base class function first
	// base class is CObject in this case
	Item::Serialize(archive);

	// now do the stuff for our specific class
	//if (archive.IsStoring())
	//	archive << m_name << m_number;
	//else
	//	archive >> m_name >> m_number;
}
