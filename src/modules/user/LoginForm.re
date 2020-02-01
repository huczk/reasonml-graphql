open ReactUtils;
open BsReform;

module StateLenses = [%lenses
  type state = {
    email: string,
    password: string,
  }
];

module LoginForm = ReForm.Make(StateLenses);

let useLoginForm = (~onSubmit) => {
  LoginForm.use(
    ~schema={
      LoginForm.Validation.Schema([|
        Email(Email),
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
    ~initialState={email: "", password: ""},
    (),
  );
};

[@react.component]
let make = () => {
  let {UserProvider.login} = UserProvider.useUser();
  let (loginMutation, simple, _full) =
    ApolloHooks.useMutation(User_Query.Login.definition);

  let {state, submit, handleChange}: LoginForm.api =
    useLoginForm(~onSubmit=({email, password}) => {
      loginMutation(
        ~variables=User_Query.Login.makeVariables(~email, ~password, ()),
        (),
      )
      |> Js.Promise.then_(
           (result: ApolloHooks.Mutation.result(User_Query.Login.t)) => {
           switch (result) {
           | Data(user) =>
             login(user##login##token);
             ReasonReactRouter.push("/");
           | Error(_)
           | NoData => ()
           };

           Js.Promise.resolve();
         })
      |> Js.Promise.catch(_ => Js.Promise.resolve())
      |> ignore
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
        "Sign in"->s
      </button>
    </form>
  </>;
};