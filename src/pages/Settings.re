open ReactUtils;

[@react.component]
let make = () => {
  let {UserProvider.logout} = UserProvider.useUser();

  <div className="settings-page">
    <div className="container page">
      <div className="row">
        <div className="col-md-6 offset-md-3 col-xs-12">
          <h1 className="text-xs-center"> "Your Settings"->s </h1>
          <UserForm />
          <hr />
          <button onClick={_ => logout()} className="btn btn-outline-danger">
            "Or click here to logout."->s
          </button>
        </div>
      </div>
    </div>
  </div>;
};