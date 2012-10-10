Automated Packaging Tool for Jubatus
====================================

1. Setup the build tools.

::

 sudo apt-get install packaging-dev

2. Modify ``package_config`` to specify which version to build. See below for details.

3. Now build the package with ``./jubapkg -uCai`` (Note: this automatically installs packages with sudo).
   Run ``./jubapkg`` to see the detailed usage.

Requirements
------------

* Ubuntu Server 12.04 LTS (x86_64)
* ``libmsgpack-dev``, ``libevent-dev``, ``libzookeeper-mt-dev``, ``libmecab-dev``, ``mecab-ipadic-utf8`` package;
  ``libmecab-dev`` must be 0.99x (see: https://launchpad.net/ubuntu/+source/mecab/0.99.3-3)

package_config Rules
====================

Each variable takes form of ``PKG_$package_VARNAME``. If the package contains dash (``-``) in the name, replace it with underscore (``_``). Empty value is not permitted.

General
-------

* ``*_VERSION``

  - Product version.

* ``*_RELEASE``

  - Package version.
  - Must be incremented to release new package with the same ``*_VERSION``.

* ``*_URL``

  - Source tarball or the repository.

* ``*_URL_TYPE``

  - Type of ``*_URL``; one of ``tar.gz``, ``tar.bz2``, ``git`` or ``hg``.

For jubatus, pficommon, re2
---------------------------

* ``*_CHECKOUT``

  - Tag, commit ID or branch of the VCS to use for this build.

* ``*_IS_SNAPSHOT``

  - When set to ``yes``, date and ID of the commit is appended to the package version. It is intended for "nightly" builds.
  - When set to ``no``, use simple package version. This is for release.

For jubatus-client
------------------

* ``*_X_ARCHIVE_DIR``

  - The name of the directory extracted from the archive specified in ``*_URL``.
