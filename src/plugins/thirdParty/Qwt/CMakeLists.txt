PROJECT(QwtPlugin)

# Retrieve Qwt's definitions

SET(DEFINITIONS
    MML_TEST
)

IF(WIN32)
    LIST(APPEND DEFINITIONS
        _SCL_SECURE_NO_WARNINGS

        QWT_DLL
    )
ENDIF()

SET(QWT_DEFINITIONS ${DEFINITIONS} PARENT_SCOPE)

# Use our pre-built version unless instructed otherwise

IF(USE_PREBUILT_QWT_PLUGIN)
    # Retrieve the plugin's binary file(s)

    STRING(REPLACE "${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/" ""
           RELATIVE_PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${REMOTE_EXTERNAL_BINARIES_DIR}")

    IF(WIN32)
        IF(RELEASE_MODE)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Qwt.dll b301433cd9155e18fccd08a0e699214d4cfc1524)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Qwt.lib a3790e5b7fd71bdf9bec1ebc3fc82c007ff8c3a0)
        ELSE()
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Qwt.dll 8e7293c83b75d6daa9409b699f7d5be814f7ea25)
            RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} Qwt.lib 7f226cfac20eb8030f01274b2f71049c8064be93)
        ENDIF()
    ELSEIF(APPLE)
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQwt.dylib 13b75c9d4a1ed4e9ad65b269e969dcfb120ffa39)
    ELSE()
        RETRIEVE_BINARY_FILE(${RELATIVE_PROJECT_SOURCE_DIR} libQwt.so 23eb2bc0bfd0a28ca9ff4301514bea26f2202e63)
    ENDIF()

    # Add the plugin as a binary
    # Note: in the case of Windows, we must refer to the distribution directory
    #       as opposed to the plugins directory since we need the .lib file and
    #       it's only there...

    IF(WIN32)
        SET(QWT_PLUGIN_BINARY ${PROJECT_SOURCE_DIR}/${LOCAL_EXTERNAL_BINARIES_DIR}/${CMAKE_IMPORT_LIBRARY_PREFIX}Qwt${CMAKE_IMPORT_LIBRARY_SUFFIX} PARENT_SCOPE)
    ELSE()
        SET(QWT_PLUGIN_BINARY ${DEST_PLUGINS_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}Qwt${CMAKE_SHARED_LIBRARY_SUFFIX} PARENT_SCOPE)
    ENDIF()

    ADD_PLUGIN_BINARY(Qwt
        INCLUDE_DIRS
            src
            textengines/mathml
    )
ELSE()
    # Add the plugin

    SET(QWT_PLUGIN Qwt PARENT_SCOPE)

    ADD_PLUGIN(Qwt
        THIRD_PARTY
        SOURCES
            ../../plugininfo.cpp

            src/qwt_abstract_legend.cpp
            src/qwt_abstract_scale.cpp
            src/qwt_abstract_scale_draw.cpp
            src/qwt_abstract_slider.cpp
            src/qwt_analog_clock.cpp
            src/qwt_arrow_button.cpp
            src/qwt_clipper.cpp
            src/qwt_color_map.cpp
            src/qwt_column_symbol.cpp
            src/qwt_compass.cpp
            src/qwt_compass_rose.cpp
            src/qwt_counter.cpp
            src/qwt_curve_fitter.cpp
            src/qwt_date.cpp
            src/qwt_date_scale_draw.cpp
            src/qwt_date_scale_engine.cpp
            src/qwt_dial.cpp
            src/qwt_dial_needle.cpp
            src/qwt_dyngrid_layout.cpp
            src/qwt_event_pattern.cpp
            src/qwt_graphic.cpp
            src/qwt_interval.cpp
            src/qwt_interval_symbol.cpp
            src/qwt_knob.cpp
            src/qwt_legend.cpp
            src/qwt_legend_data.cpp
            src/qwt_legend_label.cpp
            src/qwt_magnifier.cpp
            src/qwt_math.cpp
            src/qwt_matrix_raster_data.cpp
            src/qwt_null_paintdevice.cpp
            src/qwt_painter.cpp
            src/qwt_painter_command.cpp
            src/qwt_panner.cpp
            src/qwt_picker.cpp
            src/qwt_picker_machine.cpp
            src/qwt_pixel_matrix.cpp
            src/qwt_plot.cpp
            src/qwt_plot_abstract_barchart.cpp
            src/qwt_plot_axis.cpp
            src/qwt_plot_barchart.cpp
            src/qwt_plot_canvas.cpp
            src/qwt_plot_curve.cpp
            src/qwt_plot_dict.cpp
            src/qwt_plot_directpainter.cpp
            src/qwt_plot_glcanvas.cpp
            src/qwt_plot_grid.cpp
            src/qwt_plot_histogram.cpp
            src/qwt_plot_intervalcurve.cpp
            src/qwt_plot_item.cpp
            src/qwt_plot_layout.cpp
            src/qwt_plot_legenditem.cpp
            src/qwt_plot_magnifier.cpp
            src/qwt_plot_marker.cpp
            src/qwt_plot_multi_barchart.cpp
            src/qwt_plot_panner.cpp
            src/qwt_plot_picker.cpp
            src/qwt_plot_rasteritem.cpp
            src/qwt_plot_renderer.cpp
            src/qwt_plot_rescaler.cpp
            src/qwt_plot_scaleitem.cpp
            src/qwt_plot_seriesitem.cpp
            src/qwt_plot_shapeitem.cpp
            src/qwt_plot_spectrocurve.cpp
            src/qwt_plot_spectrogram.cpp
            src/qwt_plot_svgitem.cpp
            src/qwt_plot_textlabel.cpp
            src/qwt_plot_tradingcurve.cpp
            src/qwt_plot_xml.cpp
            src/qwt_plot_zoneitem.cpp
            src/qwt_plot_zoomer.cpp
            src/qwt_point_3d.cpp
            src/qwt_point_data.cpp
            src/qwt_point_mapper.cpp
            src/qwt_point_polar.cpp
            src/qwt_raster_data.cpp
            src/qwt_round_scale_draw.cpp
            src/qwt_sampling_thread.cpp
            src/qwt_scale_div.cpp
            src/qwt_scale_draw.cpp
            src/qwt_scale_engine.cpp
            src/qwt_scale_map.cpp
            src/qwt_scale_widget.cpp
            src/qwt_series_data.cpp
            src/qwt_slider.cpp
            src/qwt_spline.cpp
            src/qwt_symbol.cpp
            src/qwt_system_clock.cpp
            src/qwt_text.cpp
            src/qwt_text_engine.cpp
            src/qwt_text_label.cpp
            src/qwt_thermo.cpp
            src/qwt_transform.cpp
            src/qwt_wheel.cpp
            src/qwt_widget_overlay.cpp
            src/qwtplugin.cpp

            textengines/mathml/qwt_mathml_text_engine.cpp
            textengines/mathml/qwt_mml_document.cpp
            textengines/mathml/qwt_mml_entity_table.cpp
        HEADERS_MOC
            src/qwt_abstract_legend.h
            src/qwt_abstract_scale.h
            src/qwt_abstract_slider.h
            src/qwt_analog_clock.h
            src/qwt_compass.h
            src/qwt_counter.h
            src/qwt_dial.h
            src/qwt_dyngrid_layout.h
            src/qwt_knob.h
            src/qwt_legend.h
            src/qwt_legend_label.h
            src/qwt_magnifier.h
            src/qwt_panner.h
            src/qwt_picker.h
            src/qwt_plot.h
            src/qwt_plot_canvas.h
            src/qwt_plot_glcanvas.h
            src/qwt_plot_magnifier.h
            src/qwt_plot_panner.h
            src/qwt_plot_picker.h
            src/qwt_plot_renderer.h
            src/qwt_plot_zoomer.h
            src/qwt_sampling_thread.h
            src/qwt_scale_widget.h
            src/qwt_slider.h
            src/qwt_text_label.h
            src/qwt_thermo.h
            src/qwt_wheel.h
            src/qwtplugin.h
        INCLUDE_DIRS
            src
            textengines/mathml
        DEFINITIONS
            ${DEFINITIONS}
            QWT_MAKEDLL
        QT_MODULES
            Concurrent
            OpenGL
            PrintSupport
            Svg
            Widgets
            Xml
    )
ENDIF()
