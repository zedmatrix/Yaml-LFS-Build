#!/bin/sh

echo "* Preparing ${DESTDIR} Image *"

du -shBK ${DESTDIR} > ${DESTDIR}/before.txt

if [[ -d "${DESTDIR}/etc" ]]; then
	echo "* Merging ${DESTDIR}/etc"
	cp -av ${DESTDIR}/etc/* /etc
fi

if [[ -d "${DESTDIR}/var" ]]; then
	echo "* Merging ${DESTDIR}/var"
	cp -av ${DESTDIR}/var/* /var
fi

if [[ -d "${DESTDIR}/usr" ]]; then
	echo "* Making Sure Merged-usr *"
	for dir in bin lib sbin; do
		[[ -d "${DESTDIR}/${dir}" ]] && mv -v "${DESTDIR}/${dir}" "${DESTDIR}/usr/${dir}"
	done

	echo "* Stripping ${DESTDIR}/usr"
	/ybuild/ystrip "${DESTDIR}/usr"

    echo "* Merging ${DESTDIR}/usr"
	cp -av ${DESTDIR}/usr/* /usr
fi
du -shBK ${DESTDIR} > ${DESTDIR}/after.txt

echo "* Done *"
