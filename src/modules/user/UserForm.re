open ReactUtils;
open BsReform;

module StateLenses = [%lenses
  type state = {
    image: string,
    username: string,
    bio: string,
    email: string,
    password: string,
  }
];

module SettingsForm = ReForm.Make(StateLenses);

let useSettingsForm = (~onSubmit) => {
  SettingsForm.use(
    ~schema={
      SettingsForm.Validation.Schema([|Email(Email)|]);
    },
    ~onSubmit=
      ({state}) => {
        onSubmit(state.values);

        None;
      },
    ~initialState={image: "", username: "", bio: "", email: "", password: ""},
    (),
  );
};

[@react.component]
let make = () => {
  let {UserProvider.user, UserProvider.login} = UserProvider.useUser();
  let (mutation, simple, _full) =
    ApolloHooks.useMutation(User_Query.UpdateProfile.definition);

  let {state, submit, handleChange, setValues}: SettingsForm.api =
    useSettingsForm(~onSubmit=({image, username, bio, email, password}) => {
      mutation(
        ~variables=
          User_Query.UpdateProfile.makeVariables(
            ~image,
            ~username,
            ~bio,
            ~email,
            ~password,
            (),
          ),
        (),
      )
      |> Js.Promise.then_(
           (result: ApolloHooks.Mutation.result(User_Query.UpdateProfile.t)) => {
           switch (result) {
           | Data(user) =>
             login(user##updateProfile##token);
             ReasonReactRouter.push("/");
           | Error(_)
           | NoData => ()
           };

           Js.Promise.resolve();
         })
      |> ignore
    });

  React.useEffect1(
    () => {
      switch (user) {
      | LoggedIn({username, image, bio, email}) =>
        setValues({image, username, bio, email, password: ""});
        Some(ignore);
      | Anonymous => Some(ignore)
      }
    },
    [|user|],
  );

  <>
    <ul className="error-messages">
      {Errors.getGqlErrors(simple)}
      {Errors.getFormErrors(state.fieldsState)}
    </ul>
    <form onSubmit={eventPrevent(_ => submit())}>
      <fieldset>
        <fieldset className="form-group">
          <input
            className="form-control"
            type_="text"
            placeholder="URL of profile picture"
            value={state.values.image}
            onChange={handleFieldChange(handleChange(Image))}
          />
        </fieldset>
        <fieldset className="form-group">
          <input
            className="form-control form-control-lg"
            type_="text"
            placeholder="Your Name"
            value={state.values.username}
            onChange={handleFieldChange(handleChange(Username))}
          />
        </fieldset>
        <fieldset className="form-group">
          <textarea
            className="form-control form-control-lg"
            rows=8
            placeholder="Short bio about you"
            value={state.values.bio}
            onChange={handleFieldChange(handleChange(Bio))}
          />
        </fieldset>
        <fieldset className="form-group">
          <input
            className="form-control form-control-lg"
            type_="text"
            placeholder="Email"
            value={state.values.email}
            onChange={handleFieldChange(handleChange(Email))}
          />
        </fieldset>
        <fieldset className="form-group">
          <input
            className="form-control form-control-lg"
            type_="password"
            placeholder="Password"
            value={state.values.password}
            onChange={handleFieldChange(handleChange(Password))}
          />
        </fieldset>
        <button className="btn btn-lg btn-primary pull-xs-right">
          "Update Settings"->s
        </button>
      </fieldset>
    </form>
  </>;
};