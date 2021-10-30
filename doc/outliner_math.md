::: {#top}
::: {#titlearea}
+-----------------------------------------------------------------------+
| ::: {#projectname}                                                    |
| Cave Outliner                                                         |
| :::                                                                   |
+-----------------------------------------------------------------------+
:::

::: {#main-nav}
:::

::: {#MSearchSelectWindow onmouseover="return searchBox.OnSearchSelectShow()" onmouseout="return searchBox.OnSearchSelectHide()" onkeydown="return searchBox.OnSearchSelectKey(event)"}
:::

::: {#MSearchResultsWindow}
:::
:::

::: header
::: summary
[Static Public Member Functions](#pub-static-methods) \| [List of all
members](class_outliner_math-members.html)
:::

::: headertitle
::: title
OutlinerMath Class Reference
:::
:::
:::

::: contents
`#include <outlinermath.hh>`

+-----------------------------------+-----------------------------------+
| ## []                             |                                   |
| {#pub-static-methods} Static Publ |                                   |
| ic Member Functions {#static-publ |                                   |
| ic-member-functions .groupheader} |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **triangleDescribe** (const       |
| d3416480394fa1ae3cbe4ad6f72aab0e} | [OutlinerTriangle3D](clas         |
| static void                       | s_outliner_triangle3_d.html){.el} |
|                                   | &triangle, char \*buf, unsigned   |
|                                   | int bufSize)                      |
+-----------------------------------+-----------------------------------+
|                                   | Produce a string that describes a |
|                                   | triangle. Used in debugging.\     |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **triangleBoundingBox2D** (const  |
| 8414e86cc4a4e168a5926aff75b5de4e} | [OutlinerTriangle2D](clas         |
| static void                       | s_outliner_triangle2_d.html){.el} |
|                                   | &triangle,                        |
|                                   | [OutlinerBox2D]                   |
|                                   | (class_outliner_box2_d.html){.el} |
|                                   | &boundingBox)                     |
+-----------------------------------+-----------------------------------+
|                                   | Calculate the smallest bounding   |
|                                   | box that holds a given triangle   |
|                                   | (in a 2-dimension model).\        |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **triangleBoundingBox3D** (const  |
| b6b2bb0be2acdc2b9fcbdfe8a9cc6bdf} | [OutlinerTriangle3D](clas         |
| static void                       | s_outliner_triangle3_d.html){.el} |
|                                   | &triangle,                        |
|                                   | [OutlinerBox3D]                   |
|                                   | (class_outliner_box3_d.html){.el} |
|                                   | &boundingBox)                     |
+-----------------------------------+-----------------------------------+
|                                   | Calculate the smallest bounding   |
|                                   | box that holds a given triangle   |
|                                   | (in a 3-dimension model).\        |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **pointInsideTriangle2D** (const  |
| 6d0dca3240f2d97ad6d00f7973154e4f} | [OutlinerTriangle2D](clas         |
| static bool                       | s_outliner_triangle2_d.html){.el} |
|                                   | &triangle, const                  |
|                                   | [OutlinerVector2D](cl             |
|                                   | ass_outliner_vector2_d.html){.el} |
|                                   | &point)                           |
+-----------------------------------+-----------------------------------+
|                                   | Is a given point inside a         |
|                                   | triangle (in a 2-dimension        |
|                                   | model)?\                          |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **pointInsideBoundingBox2D**      |
| 256f7ce85590deccd7ccd9c7c0cbfeda} | (const                            |
| static bool                       | [OutlinerBox2D]                   |
|                                   | (class_outliner_box2_d.html){.el} |
|                                   | &box, const                       |
|                                   | [OutlinerVector2D](cl             |
|                                   | ass_outliner_vector2_d.html){.el} |
|                                   | &point)                           |
+-----------------------------------+-----------------------------------+
|                                   | Is a given point inside a         |
|                                   | bounding box (in 2-dimension      |
|                                   | model)?\                          |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **                                |
| 6492576cbe435ac0ccdfb3ae1b8a2f70} | boundingBoxIntersectsTriangle2D** |
| static bool                       | (const                            |
|                                   | [OutlinerTriangle2D](clas         |
|                                   | s_outliner_triangle2_d.html){.el} |
|                                   | &triangle, const                  |
|                                   | [OutlinerBox2D]                   |
|                                   | (class_outliner_box2_d.html){.el} |
|                                   | &box)                             |
+-----------------------------------+-----------------------------------+
|                                   | Does a triangle intersect a       |
|                                   | bounding box?\                    |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **                                |
| ca5779b5b445e27c65887622d3629988} | boundingBoxIntersectsTriangle3D** |
| static bool                       | (const                            |
|                                   | [OutlinerTriangle3D](clas         |
|                                   | s_outliner_triangle3_d.html){.el} |
|                                   | &triangle, const                  |
|                                   | [OutlinerBox3D]                   |
|                                   | (class_outliner_box3_d.html){.el} |
|                                   | &box)                             |
+-----------------------------------+-----------------------------------+
|                                   | Does a triangle intersect a       |
|                                   | bounding box?\                    |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **boundingBoxesIntersect3D**      |
| 20c993dbecea94a9b4843f3b1c3e3f75} | ([OutlinerBox3D]                  |
| static bool                       | (class_outliner_box3_d.html){.el} |
|                                   | &boundingBox1,                    |
|                                   | [OutlinerBox3D]                   |
|                                   | (class_outliner_box3_d.html){.el} |
|                                   | &boundingBox2)                    |
+-----------------------------------+-----------------------------------+
|                                   | Does a bounding box intersect     |
|                                   | another one?\                     |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **pointOnLine2D** (const          |
| a959a9730dc1dd2ce9ddd1a96ade17e6} | [OutlinerLine2D](                 |
| static bool                       | class_outliner_line2_d.html){.el} |
|                                   | &line, const                      |
|                                   | [OutlinerVector2D](cl             |
|                                   | ass_outliner_vector2_d.html){.el} |
|                                   | &point)                           |
+-----------------------------------+-----------------------------------+
|                                   | Is a given point on a line?\      |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **lineIntersectsVerticalLine2D**  |
| f0de49f0522dbb81256ec793265ca789} | (const                            |
| static bool                       | [OutlinerLine2D](                 |
|                                   | class_outliner_line2_d.html){.el} |
|                                   | &line, const                      |
|                                   | [OutlinerLine2D](                 |
|                                   | class_outliner_line2_d.html){.el} |
|                                   | &verticalLine,                    |
|                                   | [OutlinerVector2D](cl             |
|                                   | ass_outliner_vector2_d.html){.el} |
|                                   | &intersectionPoint)               |
+-----------------------------------+-----------------------------------+
|                                   | Does a line intersect a vertical  |
|                                   | line?\                            |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | *                                 |
| 96b2eae2b356b7d39cf01cac5ac38bc4} | *lineIntersectsHorizontalLine2D** |
| static bool                       | (const                            |
|                                   | [OutlinerLine2D](                 |
|                                   | class_outliner_line2_d.html){.el} |
|                                   | &line, const                      |
|                                   | [OutlinerLine2D](                 |
|                                   | class_outliner_line2_d.html){.el} |
|                                   | &horizontalLine,                  |
|                                   | [OutlinerVector2D](cl             |
|                                   | ass_outliner_vector2_d.html){.el} |
|                                   | &intersectionPoint)               |
+-----------------------------------+-----------------------------------+
|                                   | Does a line intersect a           |
|                                   | horizontal line?\                 |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **vectorTo** (const               |
| be22bf1a1c72aa49f55ac20ff8d71874} | [OutlinerVector2D](cl             |
| static void                       | ass_outliner_vector2_d.html){.el} |
|                                   | &from, const                      |
|                                   | [OutlinerVector2D](cl             |
|                                   | ass_outliner_vector2_d.html){.el} |
|                                   | &to,                              |
|                                   | [OutlinerVector2D](cl             |
|                                   | ass_outliner_vector2_d.html){.el} |
|                                   | &result)                          |
+-----------------------------------+-----------------------------------+
|                                   | Create a vector from \"from\" to  |
|                                   | \"to\", placing the result in     |
|                                   | \"result\".\                      |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+
| []{#a                             | **mathTests** (void)              |
| 4d656c09cf20006dac2cf7ca673e317b} |                                   |
| static void                       |                                   |
+-----------------------------------+-----------------------------------+
|                                   | Run module tests.\                |
+-----------------------------------+-----------------------------------+
|                                   |                                   |
+-----------------------------------+-----------------------------------+

[]{#details}

## Detailed Description {#detailed-description .groupheader}

::: textblock
The [OutlinerMath](class_outliner_math.html){.el} class is a geometry
and mathematics module. It hosts a number of static methods that can be
used to calculate bounding boxes for triangles, whether points are
inside a triangle, etc.
:::

------------------------------------------------------------------------

The documentation for this class was generated from the following file:

-   src/[outlinermath.hh](outlinermath_8hh_source.html){.el}
:::

------------------------------------------------------------------------

[Generated by [![doxygen](doxygen.svg){.footer width="104"
height="31"}](https://www.doxygen.org/index.html) 1.9.2]{.small}
