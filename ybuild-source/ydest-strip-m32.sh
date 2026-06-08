#!/bin/sh

echo "* Preparing ${DESTDIR} Image *"
if [[ -d "${DESTDIR}/usr/lib32" ]]; then
    du -shBK "${DESTDIR}/usr/lib32" > ${DESTDIR}/before.txt

	echo "* Stripping ${DESTDIR}/usr/lib32"
	/ybuild/ystrip "${DESTDIR}/usr/lib32"

    echo "* Merging ${DESTDIR}/usr/lib32"
	cp -av ${DESTDIR}/usr/lib32/* /usr/lib32

	du -shBK "${DESTDIR}/usr/lib32" > ${DESTDIR}/after.txt
else
	echo "Nothing to Do"
fi

echo "* Done *"
