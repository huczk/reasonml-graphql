open ReactUtils;
open BsReform;

module StateLenses = [%lenses
  type state = {
    username: string,
    email: string,
    password: string,
  }
];

module RegisterForm = ReForm.Make(StateLenses);

let useRegisterForm = (~onSubmit) => {
  RegisterForm.use(
    ~schema={
      RegisterForm.Validation.Schema([|
        Email(Email),
        Custom(
          Username,
          values =>
            Js.String.length(values.username) == 0
              ? Error("Username must not be empty") : Valid,
        ),
        Custom(
          Email,
          values =>
            Js.String.length(values.email) == 0
              ? Error("Email must not be empty") : Valid,
        ),
        Custom(
          Password,
          values =>
            Js.String.length(values.password) == 0
              ? Error("Password must not be empty") : Valid,
        ),
      |]);
    },
    ~onSubmit=
      ({state}) => {
        onSubmit(state.values);

        None;
      },
    ~initialState={username: "", email: "", password: ""},
    (),
  );
};

[@react.component]
let make = () => {
  let {UserProvider.login} = UserProvider.useUser();
  let (registerMutation, simple, _full) =
    ApolloHooks.useMutation(User_Query.Register.definition);

  let {state, submit, handleChange}: RegisterForm.api =
    useRegisterForm(~onSubmit=({username, email, password}) => {
      registerMutation(
        ~variables=
          User_Query.Register.makeVariables(~username, ~email, ~password, ()),
        (),
      )
      |> Js.Promise.then_(
           (result: ApolloHooks.Mutation.result(User_Query.Register.t)) => {
           switch (result) {
           | Data(user) => login(user##register##token)
           | Error(_)
           | NoData => ()
           };

           Js.Promise.resolve();
         })
      |> ignore;
      ReasonReactRouter.push("/");
    });

  <>
    <ul className="error-messages">
      {Errors.getGqlErrors(simple)}
      {Errors.getFormErrors(state.fieldsState)}
    </ul>
    <form onSubmit={eventPrevent(_ => submit())}>
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
        "Sign up"->s
      </button>
    </form>
  </>;
};