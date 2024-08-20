SET target="template_debug"

if %target%=="debug"(
	target="template_release"
)

py -m SCons platform=windows target="$target"
