/// <reference path="../pb_data/types.d.ts" />

routerAdd("POST", "/api/_core/users/from_discord", (c) => {
    // attempt to retrieve the user, if it doesn't exist, create it.
    const data = new DynamicModel({
        username: "",
        game_name: "",
        game_mode: "",
        discord_id: 0,
    });
    c.bind(data);

    try {
        const record = $app.dao().findFirstRecordByData("users", "discord_id", data.discord_id);
        return c.json(200, { "message": "found", data: record })
    } catch (e) {
        if (!e.message.includes("no rows")) throw e;
        const record = new Record($app.dao().findCollectionByNameOrId("users"), {
            wear_training: 0,
            repair_training: 0,
            l_training: 0,
            h_training: 0,
            fuel_training: 0,
            co2_training: 0,
            fuel_price: 700,
            co2_price: 120,
            accumulated_count: 0,
            load: 0.87,
            income_loss_tol: 0.1,
            fourx: false,
            role: "USER",
            verified: true,
        });
        const form = new RecordUpsertForm($app, record);
        form.loadData({
            username: data.username,
            password: data.username,
            passwordConfirm: data.username,
            game_name: data.game_name,
            game_mode: data.game_mode,
            discord_id: data.discord_id
        })
        form.submit();
        return c.json(200, { "message": "created", data: record })
    }
}
    , $apis.activityLogger($app)
    // , $apis.requireAdminAuth()
)

onModelAfterUpdate((e) => {
    console.log("user updated...", e.model.get("email"))
}, "users")