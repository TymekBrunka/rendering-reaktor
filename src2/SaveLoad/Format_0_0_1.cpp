#include <App.hpp>
#include <yyjson.h>
#include <SaveLoad/Format.hpp>
#include <stdio.h>
#include <zip.h>

bool loader_0_0_1(SavableState *state, AppMetadata *meta, abstract_file *afile) {
  char *data = NULL;
  zip_source_t *data_src = NULL;

  if (!strncmp(afile->path, "models/", sizeof("models/") - 1)) {
    fprintf(stderr, "model -> %s\n", afile->path);

    abstract_memory memory = abstract_file_read(afile);
    abstract_memory_read_data(&memory);
    abstract_file_make_real(afile, &memory);

    state->model_mgr.load_model(std::string{formated_path_2});

    abstract_memory_free(&memory);
  }

  return true;
}

bool scene_loader_0_0_1(SavableState *state, AppMetadata *meta, yyjson_val* root) {
  json_assert(yyjson_is_obj(root), "scene.json doesnt start with object\n");

  yyjson_val *objects_val = yyjson_obj_get(root, "objects");
  json_assert(objects_val != NULL, "json field 'objects' not present in scene.json\n");
  json_assert(yyjson_is_arr(objects_val), "json field 'objects' is not an array\n");
  
  json_iter_arr(objects_val, object_val, i) {
    json_assert(yyjson_is_obj(object_val), "json array 'objects' element #%lld not being json object\n", i);

    yyjson_val *name_val = yyjson_obj_get(object_val, "name");
    yyjson_val *model_val = yyjson_obj_get(object_val, "model");
    yyjson_val *position_val = yyjson_obj_get(object_val, "position");
    yyjson_val *rotation_val = yyjson_obj_get(object_val, "rotation");
    yyjson_val *scale_val = yyjson_obj_get(object_val, "scale");

    json_assert(name_val != NULL, "json array 'objects' element #%lld is missing field 'name'\n", i);
    json_assert(model_val != NULL, "json array 'objects' element #%lld is missing field 'model'\n", i);
    json_assert(position_val != NULL, "json array 'objects' element #%lld is missing field 'position'\n", i);
    json_assert(rotation_val != NULL, "json array 'objects' element #%lld is missing field 'rotation'\n", i);
    json_assert(scale_val != NULL, "json array 'objects' element #%lld is missing field 'scale'\n", i);

    json_assert(yyjson_is_str(name_val), "json array 'objects' element #%lld field 'name' is not a string\n", i);
    json_assert(yyjson_is_str(model_val), "json array 'objects' element #%lld field 'model' is not a string\n", i);
    json_assert(yyjson_is_arr(position_val), "json array 'objects' element #%lld field 'position' is not an array\n", i);
    json_assert(yyjson_is_arr(rotation_val), "json array 'objects' element #%lld field 'rotation' is not an array\n", i);
    json_assert(yyjson_is_arr(scale_val), "json array 'objects' element #%lld field 'scale' is not an array\n", i);

    float pos_x, pos_y, pos_z;
    float rot_x, rot_y, rot_z;
    float scale_x, scale_y, scale_z;
    json_assert(json_get_vector3(position_val, &pos_x, &pos_y, &pos_z), "json array 'objects' element #%lld field 'position' doesn't qualify as vector3\n", i);
    json_assert(json_get_vector3(rotation_val, &rot_x, &rot_y, &rot_z), "json array 'objects' element #%lld field 'position' doesn't qualify as vector3\n", i);
    json_assert(json_get_vector3(scale_val, &scale_x, &scale_y, &scale_z), "json array 'objects' element #%lld field 'position' doesn't qualify as vector3\n", i);

    // clang-format off
    state->objects.push_back(
      WorldObject{
        .transform = {
          .translation = Vector3{pos_x, pos_y, pos_z},
          .rotation = Vector4{rot_x, rot_y, rot_z, 0},
          .scale = Vector3{scale_x, scale_y, scale_z}
        },
        .model_ref = state->model_mgr.take_model(yyjson_get_str(model_val), state->objects.size())
      }
    );
    // clang-format on
  }
  json_iter_end();

  fprintf(stderr, "\nscene.json was read succesfully\n\n");

  return true;
}
